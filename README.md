**# Simulador de Hidrômetro (C++)**

Simulador simples de consumo de água com geração de imagem PNG de um hidrômetro estilizado, mostrando:
- Total em metros cúbicos (odômetro digital)
- Ponteiros analógicos para litros, décimos e centésimos
- Indicador de fluxo (spinner) quando há consumo
- Pressão da água (bar) e consumo instantâneo (L/s)

A cada incremento de 1 litro no total, o arquivo `hydrometer.png` é (re)gerado.

**# Estrutura do Projeto**

```
.
├── main                      # exemplo simples em console (sem render)
├── main.cpp                  # exemplo alternativo de simulação via console
├── background.png / hydrometer.png
├── src/
│   ├── external/
│   │   ├── stb_image.h
│   │   └── stb_image_write.h
│   ├── graphics/
│   │   ├── display.{h,cpp}  # dígitos 7 segmentos e odômetro
│   │   ├── drawing.{h,cpp}  # primitivas: linhas, círculos, retângulos
│   │   ├── image.{h,cpp}    # buffer RGB simples
│   │   └── text.{h,cpp}     # fonte bitmap 5x7
│   ├── hydrometer.{h,cpp}   # simulação (pressão e L/s aleatórios)
│   ├── renderer.{h,cpp}     # renderização do hidrômetro em PNG
│   └── main.cpp             # loop principal (gera hydrometer.png)
└── README.md
```

**# Parâmetros e Lógica**

- `Hydrometer(double liters_per_second_max = 2.0, double press_min = 1.0, double press_max = 4.0)`
  - Gera consumo instantâneo e pressão aleatórios a cada passo (`step(dt)`).
- `Renderer::renderHydrometerPNG(...)`
  - Desenha corpo do hidrômetro, odômetro (m³), ponteiros de litros/décimos/centésimos, pressão, consumo e selo/labels.
- `src/main.cpp`
  - Loop com `dt = 1s`; quando `floor(totalLiters)` aumenta, chama o `Renderer` para salvar `hydrometer.png`.

**# Ferramenta utilizada para o Diagrama**

O diagrama de classes do SHA 2.0 foi produzido usando PlantText.
Link oficial: [https://planttext.com](https://www.planttext.com/)
