# LSCL [under develop]
### Local Stuff Configuration Language
# Intro
-------
LSCL is an open-standard file format that uses human-readable text to represent data objects.  
Creation of this language was inspired by YAML, JSON and of cource C++ languages. Special thanks to creators of YAML-CPP and RapidJSON libraries.  
Data serialization formats such as JSON stand for representing data and transmitting it in the Internet. All these languages should be fast to parse and data representation standard should be universal. But all they lack features to use them as a config for some program on the PC (locally). This is what LSCL is standing for.  
It is designed for local usage as program config. It supports such features as:  
- links
- including files
- appending data to lists
