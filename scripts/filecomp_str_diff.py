import difflib
import re

FW1_FILEPATH = "funkar/output1_funkar.gbl"
FW2_FILEPATH = "funkar_inte/axels_funkar_inte.gbl"
DIFF_TEXTFILE_NAME = "string_diff.txt"

def extract_strings(file_path, min_len=20):
    with open(file_path, "rb") as f:
        data = f.read()
    strings = re.findall(rb"[ -~]{" + str(min_len).encode() + rb",}", data)
    return [s.decode("ascii") for s in strings]

file1_strings = extract_strings(FW1_FILEPATH)
file2_strings = extract_strings(FW2_FILEPATH)

diff_lines = list(difflib.Differ().compare(file1_strings, file2_strings))

with open(DIFF_TEXTFILE_NAME, "w") as f:
    for line in diff_lines:
        f.write(line + "\n")

    f.write("=======================================\n")
    f.write("============SYMBOL MEANINGS============\n")
    f.write("=======================================\n")
    f.write(f"+ --> Only exists in {FW2_FILEPATH}\n")
    f.write(f"- --> Only exists in {FW1_FILEPATH}\n")
    f.write(f"' ' --> Exists in both\n")
