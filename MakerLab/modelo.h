// GERADO por ia/treinar.py; nao editar manualmente.
#pragma once
inline int prever(float distancia, float movimento) {
  if (movimento <= 0.500000000f) {
    if (distancia <= 40.074440002f) {
      return 3;
    } else {
      return 0;
    }
  } else {
    if (distancia <= 149.913566589f) {
      if (distancia <= 39.976537704f) {
        return 3;
      } else {
        return 2;
      }
    } else {
      return 1;
    }
  }
}
