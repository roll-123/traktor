"""One-shot: strip NVIDIA Aftermath (gated SDK, not vendored) from the
Traktor.Render.Vrfy project in TraktorWin64.xms for the GearUp engine bump.

Aftermath powers debug GPU crash-dumps in the Vrfy validation render system,
which the port never selects. The code is already disabled (T_USE_AFTERMATH
undefined in RenderSystemVrfy.cpp); this removes the matching build-config
references so the linker stops pulling GFSDK_Aftermath_Lib.x64.lib (LNK1181)
and the post-build *.dll copy. Idempotent.
"""
import pathlib

p = pathlib.Path(r"D:\code\traktor\resources\build\TraktorWin64.xms")
lines = p.read_text(encoding="utf-8").splitlines(keepends=True)

SINGLE = {
    "<item>$(AFTERMATH_SDK)/include</item>",
    "<item>$(AFTERMATH_SDK)/lib/x64</item>",
    "<item>GFSDK_Aftermath_Lib.x64.lib</item>",
}
AGG_SRC = "<sourceFile>$(AFTERMATH_SDK)/lib/x64/*.dll</sourceFile>"

out, i, n_single, n_agg = [], 0, 0, 0
while i < len(lines):
    line = lines[i]
    if AGG_SRC in line:
        # Drop the whole 4-line AggregationItem block: opener (already appended),
        # this sourceFile, the targetPath, and the closing </item>.
        if out and out[-1].strip().startswith('<item type="traktor.sb.AggregationItem">'):
            out.pop()
        i += 1
        if i < len(lines) and "<targetPath>" in lines[i]:
            i += 1
        if i < len(lines) and lines[i].strip() == "</item>":
            i += 1
        n_agg += 1
        continue
    if line.strip() in SINGLE:
        n_single += 1
        i += 1
        continue
    out.append(line)
    i += 1

p.write_text("".join(out), encoding="utf-8")
print(f"removed single-line aftermath items: {n_single}")
print(f"removed aftermath dll aggregation blocks: {n_agg}")
