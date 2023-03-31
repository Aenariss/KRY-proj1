import subprocess

file = open('ciphertext.csv')
lines = file.readlines()[4:]
for line in lines:
    line = line[7:]
    process = subprocess.run(['./kry', '-c', line, '-o', 'out_file'])