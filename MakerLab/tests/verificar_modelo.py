"""Teste real do cabecalho C++ gerado, sem alegar execucao ESP32."""
from pathlib import Path
import csv
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as d:
    p = Path(d)
    source = '#include <iostream>\n#include "modelo.h"\nint main(){float d,m;while(std::cin>>d>>m) std::cout<<prever(d,m)<<"\\n";}\n'
    (p/'main.cpp').write_text(source)
    subprocess.run(['g++','-std=c++11','-I',str(root),str(p/'main.cpp'),'-o',str(p/'test')],check=True)
    with (root/'ia'/'teste.csv').open() as f: rows=list(csv.DictReader(f))
    scenarios=[(250,0,0),(250,1,1),(100,0,0),(100,1,2),(20,0,3),(20,1,3)]
    inputs = ''.join(f"{r['distancia_cm']} {r['movimento_recente']}\n" for r in rows)
    inputs += ''.join(f'{d} {m}\n' for d,m,_ in scenarios)
    outputs = subprocess.run([str(p/'test')],input=inputs,text=True,capture_output=True,check=True).stdout.splitlines()
    expected=[int(r['classe']) for r in rows]+[c for _,_,c in scenarios]
    assert list(map(int,outputs)) == expected
    print(f'OK: inferencia C++ em {len(rows)} amostras e {len(scenarios)} cenarios.')
