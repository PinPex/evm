all: code

code: .FORCE
	g++ code.cpp mySimpleComputer.cpp myTerm.cpp myBigChars.cpp myReadKey.cpp disk.cpp -o code

.FORCE:
