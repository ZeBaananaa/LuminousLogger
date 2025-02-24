import os

main_file = "main.cpp"
source_dir = "Code/Src"
header_dir = "Code/Include"
output_file = "sources.cmake"

# Get all .cpp and .hpp files
cpp_files: [str] = []
cpp_files.insert(0, main_file)

for dp, dn, filenames in os.walk(source_dir):
    for f in filenames:
        if f.endswith(".cpp"):
            cpp_files.append(os.path.join(dp, f).replace("\\", "/"))

hpp_files: [str] = []
for dp, dn, filenames in os.walk(header_dir):
    for f in filenames:
        if f.endswith(".hpp"):
            hpp_files.append(os.path.join(dp, f).replace("\\", "/"))

# Converts path to cmake format
cpp_list = "\n    ". join(cpp_files)
hpp_list = "\n    ".join(hpp_files)

# Write inside the `sources.cmake` file
with open(output_file, "w") as f:
    f.write(f"set(LL_SOURCES\n    {cpp_list})\n\n")
    f.write(f"set(LL_HEADERS\n    {hpp_list})\n")

print(f"File {output_file} successfully generated with {len(cpp_files)} sources and {len(hpp_files)} headers.")