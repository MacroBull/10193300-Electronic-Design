#! /usr/bin/python
a3 = 440
c4 = a3 * 2 ** (3. / 12)

notes = [c4*2**(i/12.)for i in range(12)]
notes = notes[0:1] + notes[2:3] + notes[4:6] + notes[7:8] + notes[9:10] + notes[11:12]

for i in range(3):
	print 4+i,' [',
	for n in notes:
		print int(n*2**i+.5), '\t',
	print ']'
