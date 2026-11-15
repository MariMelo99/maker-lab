# Plano de verificação

## Realizado localmente

- Treinamento com 6.000 amostras e avaliação em 2.000 amostras sintéticas independentes.
- Inferência do cabeçalho C++ comparada aos rótulos do conjunto de teste.
- Cenários representativos da árvore exportada.
- Estrutura JSON e referências de componentes do diagrama verificadas por script local. Isso não equivale ao lint do Wokwi.

## Executar e registrar no Wokwi

Todos os casos abaixo estão **pendentes de execução on-line**. Após testar, registrar resultado observado, data e link do vídeo ou captura. Não marcar como aprovado sem executar.

| Caso | Ambiente simulado | Resultado esperado |
|---|---|---|
| T01 | Início: 250 cm, PIR sem movimento | Espera, LED azul |
| T02 | 250 cm, clicar Simulate Motion | Aproximação, LED azul |
| T03 | 100 cm, movimento recente | Acolhimento, LED verde, menu |
| T04 | 100 cm, enviar 1 / 2 / 3 | Orientações correspondentes no serial |
| T05 | 20 cm, com ou sem movimento | Muito perto, LED vermelho |
| T06 | 250 cm e aguardar saída PIR baixa + 10 s | Retorno à espera |
| T07 | Manter botão de falha pressionado | Falha, LED vermelho, menu bloqueado |
| T08 | Soltar falha, 100 cm, reativar PIR | Recuperação para acolhimento |
| T09 | 100 cm sem movimento por mais de 10 s após PIR baixar | Espera: objeto ou pessoa imóvel não confirmada |
| T10 | Espera, enviar 1 no serial | Menu indisponível |
| T11 | Desconectar ECHO virtualmente e iniciar | Timeout, falha de leitura |
| T12 | V1: false, 100 cm sem PIR; depois V2: true, repetir | V1 acolhe; V2 aguarda movimento |

O controle ultrassônico varia de 2 a 400 cm. O botão simula indisponibilidade explicitamente; a distância real medida continua visível no serial com `falha_injetada=1`. T11 exercita separadamente a ausência de eco.

## Limites de validação

O teste local C++ verifica o modelo exportado; não valida bibliotecas Arduino, LCD, pinagem, compilação ESP32 ou integração no simulador. Os limites de classe aprendidos têm pequenas diferenças em relação aos limiares de rotulagem; use 20, 100 e 250 cm na demonstração e documente testes próximos de 40 e 150 cm se realizados.
