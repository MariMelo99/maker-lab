# Roteiro — até 6 minutos

Gravar depois de executar os testes no Wokwi. Mostrar o navegador, os sensores, LCD, LEDs e monitor serial. Usar dados fictícios.

| Tempo | Mostrar e explicar |
|---|---|
| 0:00–0:40 | Problema: acolhimento e orientação em pontos de apoio à busca de desaparecidos; apresentar Reencontro e integrantes |
| 0:40–1:15 | Explicar que não havia robô anterior; baseline V1 e evolução V2 foram criadas nesta entrega; apresentar ESP32, ultrassom e PIR adicional |
| 1:15–1:55 | V1 com `false`: objeto próximo ativa acolhimento. V2 com `true`: aproximar sem PIR mantém espera; ativar PIR inicia acolhimento |
| 1:55–2:50 | Afastar para 250 cm e ativar movimento, aproximar para 100 cm, mostrar LCD/LEDs e menu; enviar opções no serial |
| 2:50–3:30 | Mostrar 20 cm e alerta; retornar a 250 cm e aguardar a janela para espera automática |
| 3:30–4:10 | Pressionar falha, mostrar bloqueio de atendimento e recuperação; explicar tratamento de eco ausente |
| 4:10–5:05 | Mostrar treino, teste, árvore e `modelo.h`. Explicar inferência local e que os rótulos são sintéticos; 100% no teste artificial não representa precisão de campo |
| 5:05–5:40 | Abrir manualmente portal de apoio; esclarecer que a simulação apenas orienta, sem consultar dados ou identificar pessoas |
| 5:40–5:55 | Mostrar README, links e testes; explicar limitação do PIR para pessoa imóvel e evolução futura |

Se a compilação consumir tempo, pausar a gravação ou preparar duas abas para a comparação V1/V2. A fala deve acompanhar o que realmente aparece no vídeo.
