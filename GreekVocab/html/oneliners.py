import os;
import uuid;

ldir = os.listdir();
for s in ldir:
	ext = s[len(s) - 3:];
	if (ext == "htm") :
		uid = uuid.uuid4();
		str = "{0}{1}{2}".format('<ComponentRef Id="',s,'" />');
		print(str);

