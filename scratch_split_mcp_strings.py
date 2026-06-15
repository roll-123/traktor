"""Split oversized wide raw-string literals in the MCP NodeEmitterCapture.cpp so
they compile under MSVC (C2026: string literals capped at ~16 KB). The new MCP
server was only tested on Linux/Clang; MSVC rejects the largest embedded node
emitter sources. Adjacent C++ string literals concatenate, so we break each
LR"NODESRC(...)NODESRC" literal that exceeds the cap into several adjacent
literals at newline boundaries — content is byte-for-byte preserved. Idempotent
enough for a one-shot; re-running just splits already-small pieces no further.
"""
import re
import pathlib

PATH = pathlib.Path(r"D:\code\traktor\code\MCP\Server\Editor\NodeEmitterCapture.cpp")
MAXLEN = 7000  # wide (L"...") chars cost 2 bytes each; 7000*2=14000 B < MSVC's 16384 B cap

text = PATH.read_text(encoding="utf-8")
pat = re.compile(r'LR"NODESRC\((.*?)\)NODESRC"', re.DOTALL)

split_count = 0
chunk_count = 0

def split_content(content):
    """Yield chunks of `content`, each <= MAXLEN, preferring newline boundaries."""
    i, n = 0, len(content)
    while i < n:
        if n - i <= MAXLEN:
            yield content[i:]
            return
        # find last newline within the window
        window_end = i + MAXLEN
        nl = content.rfind("\n", i, window_end)
        cut = nl + 1 if nl > i else window_end  # mid-line fallback
        yield content[i:cut]
        i = cut

def repl(m):
    global split_count, chunk_count
    content = m.group(1)
    if len(content) <= MAXLEN:
        return m.group(0)
    chunks = list(split_content(content))
    split_count += 1
    chunk_count += len(chunks)
    return " ".join(f'LR"NODESRC({c})NODESRC"' for c in chunks)

new_text = pat.sub(repl, text)
PATH.write_text(new_text, encoding="utf-8")

# Report largest remaining literal as a sanity check.
maxlit = max((len(g) for g in pat.findall(new_text)), default=0)
print(f"oversized literals split: {split_count} (into {chunk_count} chunks)")
print(f"largest literal now: {maxlit} chars (cap ~16380)")
