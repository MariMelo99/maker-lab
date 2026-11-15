"""Treina e exporta uma arvore pequena para o ESP32. Dados sinteticos, nao reais."""
from pathlib import Path
import csv
import json
import numpy as np
from sklearn.tree import DecisionTreeClassifier, export_text
from sklearn.metrics import accuracy_score, confusion_matrix

ROOT = Path(__file__).resolve().parents[1]
LABELS = ['ESPERA', 'APROXIMACAO', 'ACOLHIMENTO', 'MUITO_PERTO']

def politica(d, m):
    if d < 40: return 3
    if d <= 150: return 2 if m else 0
    return 1 if m else 0

def gerar(seed, n):
    rng = np.random.default_rng(seed)
    x = np.column_stack([rng.uniform(2, 400, n), rng.integers(0, 2, n)])
    y = np.array([politica(d, int(m)) for d, m in x])
    return x, y

def main():
    x, y = gerar(42, 6000)
    xt, yt = gerar(2026, 2000)
    model = DecisionTreeClassifier(max_depth=4, random_state=42).fit(x, y)
    tree = model.tree_
    def code(node, depth=1):
        pad = '  ' * depth
        if tree.children_left[node] == tree.children_right[node]:
            return pad + f'return {int(model.classes_[tree.value[node][0].argmax()])};\n'
        feature = ['distancia', 'movimento'][tree.feature[node]]
        threshold = float(tree.threshold[node])
        return (pad + f'if ({feature} <= {threshold:.9f}f) {{\n'
                + code(tree.children_left[node], depth+1) + pad + '} else {\n'
                + code(tree.children_right[node], depth+1) + pad + '}\n')
    header = ('// GERADO por ia/treinar.py; nao editar manualmente.\n#pragma once\n'
              'inline int prever(float distancia, float movimento) {\n' + code(0) + '}\n')
    (ROOT/'modelo.h').write_text(header)
    for name, xx, yy in [('treino',x,y), ('teste',xt,yt)]:
        with (ROOT/'ia'/f'{name}.csv').open('w',newline='') as f:
            w = csv.writer(f); w.writerow(['distancia_cm','movimento_recente','classe'])
            for row, label in zip(xx,yy): w.writerow([float(row[0]),int(row[1]),int(label)])
    predicted = model.predict(xt)
    metrics = {'origem':'Dados sinteticos rotulados por politica de projeto; sem validacao de campo',
               'treino':len(y),'teste':len(yt),'seed_treino':42,'seed_teste':2026,
               'acuracia_sintetica':float(accuracy_score(yt,predicted)),
               'classes':LABELS,'matriz_confusao':confusion_matrix(yt,predicted,labels=[0,1,2,3]).tolist(),
               'profundidade':model.get_depth(),'nos':tree.node_count}
    (ROOT/'ia'/'metricas.json').write_text(json.dumps(metrics,indent=2,ensure_ascii=False))
    (ROOT/'ia'/'arvore.txt').write_text(export_text(model,feature_names=['distancia','movimento']))
    print(json.dumps(metrics,ensure_ascii=False))

if __name__ == '__main__': main()
