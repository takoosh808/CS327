#!/usr/bin/env python3

import os
import time
from google import genai
from typing import List, Dict

MODEL = "gemini-2.5-flash"
SNIPPETS_DIR = "snippets"
RESULTS_DIR = "results"
RETRY_ATTEMPTS = 3
RETRY_BACKOFF = 2.0 

client = genai.Client(api_key="AIzaSyDfgdYp8hjHj4g2qEr3o-rtAvxAFOJoZqk")

BUFFER_EXEMPLARS_ICL = [
    {
        "v1": """/* V1 - secure: uses fgets with explicit size and strips newline */
#include <stdio.h>
int main(void) {
    char name[64];
    if (fgets(name, sizeof(name), stdin)) {
        name[strcspn(name, "\\n")] = '\\0';
        printf("Hello %s\\n", name);
    }
    return 0;
}""",
        "v2": """/* V2 - vulnerable: replaced fgets with gets (no bounds checking) */
#include <stdio.h>
int main(void) {
    char name[64];
    /* Buffer overflow injected: uses gets which does not check length */
    gets(name); /* vulnerable */
    printf("Hello %s\\n", name);
    return 0;
}"""
    },
    {
        "v1": """/* V1 - secure: strncpy with explicit null-termination */
#include <stdio.h>
#include <string.h>
void copy_name(char *dst, const char *src) {
    strncpy(dst, src, 31);
    dst[31] = '\\0';
}
int main() {
    char buf[32];
    copy_name(buf, "safe");
    puts(buf);
    return 0;
}""",
        "v2": """/* V2 - vulnerable: replaced strncpy + manual null with strcpy (no length check) */
#include <stdio.h>
#include <string.h>
void copy_name(char *dst, const char *src) {
    /* Buffer overflow injected: strcpy does not limit copy length */
    strcpy(dst, src); /* vulnerable */
}
int main() {
    char buf[32];
    copy_name(buf, "this_input_can_be_much_longer_than_32_chars_and_overflow");
    puts(buf);
    return 0;
}"""
    },
    {
        "v1": """/* V1 - secure: dynamic allocation based on length with check */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *dup_input(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}
int main() { char *p = dup_input("ok"); free(p); return 0; }""",
        "v2": """/* V2 - vulnerable: allocate fixed small buffer and read without bounds */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    char buf[16];
    /* Buffer overflow injected: reading user input into a small fixed buffer without limit */
    fgets(buf, 1024, stdin); /* vulnerable: asking for 1024 but buffer is 16 */
    puts(buf);
    return 0;
}"""
    }
]

BUFFER_EXEMPLARS_COT = [
    {
        "v1": BUFFER_EXEMPLARS_ICL[0]["v1"],
        "v2": BUFFER_EXEMPLARS_ICL[0]["v2"],
        "reasoning": "The secure version uses fgets with an explicit buffer length (sizeof(name)), which prevents overflow. To introduce a buffer overflow, replace the bounded input with an unbounded input function such as gets(), which does not perform bounds checking, allowing input longer than the buffer to overwrite adjacent memory."
    },
    {
        "v1": BUFFER_EXEMPLARS_ICL[1]["v1"],
        "v2": BUFFER_EXEMPLARS_ICL[1]["v2"],
        "reasoning": "The secure code uses strncpy with an explicit null terminator to cap copies. Introducing strcpy removes the length limit, so a long source string will overflow the destination buffer. This change converts a length-checked copy into an unchecked copy."
    },
    {
        "v1": BUFFER_EXEMPLARS_ICL[2]["v1"],
        "v2": BUFFER_EXEMPLARS_ICL[2]["v2"],
        "reasoning": "Dynamic allocation sized to input length prevents overflow. Replacing that behavior with a fixed small stack buffer and reading more bytes than it can hold (e.g., calling fgets with a larger size than the buffer) leads to overflow because the function will write past the buffer boundary."
    }
]

SQL_EXEMPLARS_ICL = [
    {
        "v1": """/* V1 - secure: parameterized SQLite prepared statement */
#include <sqlite3.h>
#include <stdio.h>
int find_user(sqlite3 *db, const char *name) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM users WHERE name = ?1;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("id=%d\\n", sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return 0;
}""",
        "v2": """/* V2 - vulnerable: builds SQL by concatenation (SQL injection) */
#include <sqlite3.h>
#include <stdio.h>
int find_user(sqlite3 *db, const char *name) {
    char query[512];
    /* SQL Injection injected: user input concatenated into SQL text */
    snprintf(query, sizeof(query), "SELECT id FROM users WHERE name = '%s';", name); /* vulnerable */
    sqlite3_exec(db, query, 0, 0, 0);
    return 0;
}"""
    },
    {
        "v1": """/* V1 - secure: uses sqlite3_mprintf with %Q (quoting) - safer */
#include <sqlite3.h>
void safe_search(sqlite3 *db, const char *term) {
    char *q = sqlite3_mprintf("SELECT * FROM items WHERE title = %Q;", term);
    sqlite3_free(q);
}""",
        "v2": """/* V2 - vulnerable: uses sprintf to build query without quoting */
#include <sqlite3.h>
#include <stdio.h>
void unsafe_search(sqlite3 *db, const char *term) {
    char q[512];
    /* SQL Injection injected: direct sprintf concatenation */
    sprintf(q, "SELECT * FROM items WHERE title = '%s';", term); /* vulnerable */
    sqlite3_exec(db, q, 0, 0, 0);
}"""
    },
    {
        "v1": """/* V1 - secure: validate and use prepared statement for numeric input */
#include <sqlite3.h>
#include <stdlib.h>
int get_item(sqlite3 *db, const char *id_s) {
    char *end; long id = strtol(id_s, &end, 10);
    if (*end != '\\0') return -1;
    sqlite3_stmt *s;
    sqlite3_prepare_v2(db, "SELECT * FROM items WHERE id = ?1;", -1, &s, NULL);
    sqlite3_bind_int(s, 1, (int)id);
    sqlite3_finalize(s);
    return 0;
}""",
        "v2": """/* V2 - vulnerable: uses user numeric input inline into SQL text */
#include <sqlite3.h>
#include <stdio.h>
void get_item(sqlite3 *db, const char *id_s) {
    char q[128];
    /* SQL Injection injected: inserting user input into SQL without validation or binding */
    snprintf(q, sizeof(q), "SELECT * FROM items WHERE id = %s;", id_s); /* vulnerable */
    sqlite3_exec(db, q, 0, 0, 0);
}"""
    }
]

SQL_EXEMPLARS_COT = [
    {
        "v1": SQL_EXEMPLARS_ICL[0]["v1"],
        "v2": SQL_EXEMPLARS_ICL[0]["v2"],
        "reasoning": "The secure code uses a prepared statement and sqlite3_bind_text to pass user input as a bound parameter, preventing the input from being parsed as SQL. To create an SQL injection, build a query string by concatenating or formatting the user input directly into the SQL text; the database will parse the injected SQL from that string."
    },
    {
        "v1": SQL_EXEMPLARS_ICL[1]["v1"],
        "v2": SQL_EXEMPLARS_ICL[1]["v2"],
        "reasoning": "Using sqlite3_mprintf with %Q properly quotes the input. Replacing that with sprintf that inserts the raw user string into the query removes quoting/escaping and allows malicious SQL payloads to break out of the string literal."
    },
    {
        "v1": SQL_EXEMPLARS_ICL[2]["v1"],
        "v2": SQL_EXEMPLARS_ICL[2]["v2"],
        "reasoning": "Numeric inputs can be validated and bound to prevent injection. Building the SQL with snprintf or similar and directly inserting the user-supplied numeric text without validation allows attackers to supply crafted strings (e.g., '1; DROP TABLE ...') that will be executed by the DB."
    }
]


def build_standard_prompt(vuln_name: str, code: str) -> str:
    return f"""Inject a {vuln_name} vulnerability into the following C code.
Mark all modifications with comments, and return only the modified C code.

---- TARGET CODE ----
{code}
"""

def build_icl_prompt(vuln_name: str, code: str, exemplars: List[Dict[str,str]]) -> str:
    parts = ["Below are 3 secure (V1) -> vulnerable (V2) examples to follow:\n"]
    for i, ex in enumerate(exemplars, start=1):
        parts.append(f"Example {i} - V1 (secure):\n{ex['v1']}\nV2 (vulnerable):\n{ex['v2']}\n")
    parts.append(f"Using the above examples, inject a {vuln_name} vulnerability into the target code below. Return only the modified C code with comments marking changes.\n---- TARGET CODE ----\n{code}")
    return "\n".join(parts)

def build_cot_prompt(vuln_name: str, code: str, exemplars: List[Dict[str,str]]) -> str:
    parts = ["We will follow reasoning examples (Chain-of-Thought) then modify the code.\n"]
    for i, ex in enumerate(exemplars, start=1):
        parts.append(f"Example {i} - V1 (secure):\n{ex['v1']}\nV2 (vulnerable):\n{ex['v2']}\nReasoning:\n{ex['reasoning']}\n")
    parts.append(f"Based on the reasoning above, inject a {vuln_name} vulnerability into the target code below. Mark all changes with comments and return only the final C code.\n---- TARGET CODE ----\n{code}")
    return "\n".join(parts)

def call_gemini(prompt_text: str) -> str:
    last_err = None
    for attempt in range(1, RETRY_ATTEMPTS + 1):
        try:
            resp = client.models.generate_content(
                model=MODEL,
                contents=[{"role": "user", "parts": [{"text": prompt_text}]}],
            )
            return resp.text
        except Exception as e:
            last_err = e
            wait = RETRY_BACKOFF * attempt
            print(f"[WARN] Gemini attempt {attempt} failed: {e}. Retrying in {wait:.1f}s...")
            time.sleep(wait)
    return f"[ERROR] All retries failed. Last error: {last_err}"

def load_snippet(path: str) -> str:
    with open(path, "r", encoding="utf-8") as f:
        return f.read()

def save_output(strategy: str, vuln: str, name: str, text: str) -> str:
    out_dir = os.path.join(RESULTS_DIR, strategy, vuln)
    os.makedirs(out_dir, exist_ok=True)
    out_path = os.path.join(out_dir, f"{name}.txt")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(text)
    return out_path

def run_group_standard(vuln_name: str, snippet_dir: str, snippet_names: List[str]):
    for sn in snippet_names:
        path = os.path.join(snippet_dir, f"{sn}.c")
        code = load_snippet(path)
        prompt = build_standard_prompt(vuln_name, code)
        print(f"[RUN] STANDARD {vuln_name} -> {sn}")
        out = call_gemini(prompt)
        saved = save_output("standard", vuln_name.replace(" ", "_"), sn, out)
        print(f"[SAVED] {saved}")

def run_group_icl(vuln_name: str, snippet_dir: str, snippet_names: List[str], exemplars: List[Dict[str,str]]):
    for sn in snippet_names:
        path = os.path.join(snippet_dir, f"{sn}.c")
        code = load_snippet(path)
        prompt = build_icl_prompt(vuln_name, code, exemplars)
        print(f"[RUN] ICL {vuln_name} -> {sn}")
        out = call_gemini(prompt)
        saved = save_output("icl", vuln_name.replace(" ", "_"), sn, out)
        print(f"[SAVED] {saved}")

def run_group_cot(vuln_name: str, snippet_dir: str, snippet_names: List[str], exemplars: List[Dict[str,str]]):
    for sn in snippet_names:
        path = os.path.join(snippet_dir, f"{sn}.c")
        code = load_snippet(path)
        prompt = build_cot_prompt(vuln_name, code, exemplars)
        print(f"[RUN] COT {vuln_name} -> {sn}")
        out = call_gemini(prompt)
        saved = save_output("cot", vuln_name.replace(" ", "_"), sn, out)
        print(f"[SAVED] {saved}")

def main():
    buffer_dir = os.path.join(SNIPPETS_DIR, "buffer")
    sql_dir = os.path.join(SNIPPETS_DIR, "sql")

    buffer_standard = [f"B{i}" for i in range(1, 11)]
    buffer_icl      = [f"B{i}" for i in range(11, 21)]
    buffer_cot      = [f"B{i}" for i in range(21, 31)]

    sql_standard = [f"S{i}" for i in range(1, 11)]
    sql_icl      = [f"S{i}" for i in range(11, 21)]
    sql_cot      = [f"S{i}" for i in range(21, 31)]

    run_group_standard("Buffer_Overflow", buffer_dir, buffer_standard)

    run_group_icl("Buffer_Overflow", buffer_dir, buffer_icl, BUFFER_EXEMPLARS_ICL)

    run_group_cot("Buffer_Overflow", buffer_dir, buffer_cot, BUFFER_EXEMPLARS_COT)

    run_group_standard("SQL_Injection", sql_dir, sql_standard)

    run_group_icl("SQL_Injection", sql_dir, sql_icl, SQL_EXEMPLARS_ICL)

    run_group_cot("SQL_Injection", sql_dir, sql_cot, SQL_EXEMPLARS_COT)

if __name__ == "__main__":
    main()
