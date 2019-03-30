import os;
import uuid;

line1 = 'C:\\ProgramData\\vocablists\\';
start = '<Component Id="';
end1 = '" Guid="';
end2 = '">';
line3 = "</Component>";
line2a = '<File Id="'	
line2b = '" Source="C:\\Users\\Matthew\\Documents\\Visual Studio 2013\\Projects\\GreekVocab\\GreekVocab\\html\\';
line2c = '" KeyPath="yes"/>';
ldir = os.listdir();
for s in ldir:
	ext = s[len(s) - 3:];
	if (ext == "htm") :
		uid = uuid.uuid4();
		str = "{0}{1}{2}{3}{4}".format(start,s,end1,uid,end2);
		print(str);
		print(line2a + s + line2b + s + line2c);
		print( line3);

