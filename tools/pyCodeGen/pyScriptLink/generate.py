import xml.etree.ElementTree as ET
import os

# Clear console on startup
os.system('cls' if os.name=='nt' else 'clear')

# load the xml
tree = ET.parse('enums.xml')
root = tree.getroot()

# get xml sections
settings = root.find("settings")
includes = root.find("include")
disclaimer = root.find("disclaimer")
license = root.find("license")
enums = root.find("enums")

# the output
output = ""

## create build content ##
output += "#ifndef "+settings.find("definitionName").text+"\n"
output += "#define "+settings.find("definitionName").text+"\n"

output += "\n"

#includes
for i in includes.iter("i"):
    n = i.get("name")
    if n[0] == "!":
        n = "<" + n[1:] + ">"
    output += "#include "+n+"\n"

output += "\n"

#disclaimer
if settings.find("useDisclaimer").text == "true":
    for l in disclaimer.iter("l"):
        output += "// "+l.text+"\n"

output += "\n"

# license
if settings.find("useLicense").text == "true":
    for l in license.iter("l"):
        output += "/// "+l.text+"\n"

output += "\n"

#namespace
output += "namespace "+settings.find("namespace").text +" {\n"

#enums
for e in enums.iter("enum"):
    output +="\t// Enum: " + e.get("name") + " \\\\ \n"
    # enum
    output +="\tenum class "+settings.find("enumPrefix").text+e.get("name")+" { "
    flgE1 = False
    for v in e.iter("value"):
        if flgE1:
            output += ", "
        else:
            flgE1 = True
        output += v.text
    output+=" };\n"

    # class
    output +="\tclass "+settings.find("classPrefix").text + e.get("name")+" {\n"

    output +="\t\ttemplate<int i>\n"
    output +="\t\tstd::string get() {\n"
    for v in e.iter("value"):
        output+="\t\t\tif ( i == "+settings.find("enumPrefix").text + e.get("name")+"::"+v.text+" )\n"
        output+="\t\t\t\treturn \""+ v.text + "\";\n"
    output +="\t\t\tthrow new out_of_range::exception();\n"
    output +="\t\t}\n\n"

    output +="\t\ttemplate<unsigned char* str>\n"
    output +="\t\t"+settings.find("enumPrefix").text+e.get("name")+" get() {\n"
    for v in e.iter("value"):
        output+="\t\t\tif ( str == \""+v.text+"\" )\n"
        output+="\t\t\t\treturn "+ settings.find("enumPrefix").text + e.get("name")+"::" + v.text + ";\n"
    output +="\t\t\tthrow new out_of_range::exception();\n"
    output +="\t\t}\n\n"

    output +="\t};\n\n"

output += "}\n"

output +="\n"

# end define
output += "#endif // !"+settings.find("definitionName").text


print(output)
# file = open("../../../src/"+settings.find("filePath").text+"/"+settings.find("fileName").text,"w")
file = open("./"+settings.find("filePath").text+"/"+settings.find("fileName").text,"w")
file.write(output);
file.close();
#for enum in enums.iter("enum"):
#    print("Enum: "+enum.get("name"))
#    for value in enum.iter("value"):
#        print("\tvalue: "+value.text)
#    print("")
