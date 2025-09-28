**# Simulador de Hidrômetro (C++)**

Simulador simples de consumo de água com geração de imagem PNG de um hidrômetro estilizado, mostrando:
- Total em metros cúbicos (odômetro digital)
- Ponteiros analógicos para litros, décimos e centésimos
- Indicador de fluxo (spinner) quando há consumo
- Pressão da água (bar) e consumo instantâneo (L/s)

A cada incremento de 1 litro no total, o arquivo `hydrometer.png` é (re)gerado.

<p align="center"> ___________________________________________________ </p>

**# Estrutura do Projeto**

```
.
├── main                      # exemplo simples em console (sem render)
├── main.cpp                  # exemplo alternativo de simulação via console
├── background.png / hydrometer.png
├── medicoes_hidrometros # armazena as imagens geradas pelo simulador
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
<p align="center"> ___________________________________________________ </p>

**# Parâmetros e Lógica**

- `Hydrometer(double liters_per_second_max = 2.0, double press_min = 1.0, double press_max = 4.0)`
  - Gera consumo instantâneo e pressão aleatórios a cada passo (`step(dt)`).
- `Renderer::renderHydrometerPNG(...)`
  - Desenha corpo do hidrômetro, odômetro (m³), ponteiros de litros/décimos/centésimos, pressão, consumo e selo/labels.
- `src/main.cpp`
  - Loop com `dt = 1s`; quando `floor(totalLiters)` aumenta, chama o `Renderer` para salvar `hydrometer.png`.

<p align="center"> ___________________________________________________ </p>

**# Ferramenta utilizada para o Diagrama**

O diagrama de classes do SHA 2.0 foi produzido usando PlantText.
Link oficial: [https://planttext.com](https://www.planttext.com/)

<p align="center"> ___________________________________________________ </p>

**# Alterações - SHA (Simulador de Hidrômetro Analógico) versão 3**

Na v3, o projeto é reformulado para atender à demanda de exibir cinco hidrômetros simultâneos, e não apenas um. As alterações realizadas foram, na verdade, uma reestruturação completa do main.cpp, para introduzir o paralelismo e a simulação de múltiplos hidrômetros.

<h4>➔ Estrutura e Inclusões</h4>
<ul>
    <li>Inclusões</li>
  <ul>
    <li>#include <vector>, #include <thread>, #include <tuple>, #include <algorithm>: permite a criação e gerenciamento de múltiplas threads e a configuração de múltiplos hidrômetros usando std::vector e std::tuple.; </li>
    <li>Novo diretório: criação de um novo diretório, nomeado de 'medicoes_hidrometros', para armazenar as imagens geradas pelo simulador. </li></ul>
  <li>Globalização</li>
  <ul>
    <li>Renderer globalRenderer: cria uma única instância do Renderer fora das threads. Embora cada thread renderize um arquivo diferente, a instância é compartilhada; </li></ul>
</ul> 

<h4>➔ Migração da Lógica Principal para Threads</h4>
<ul>
    <li>A função main() anterior, que continha o loop while(true), foi dividida em duas partes:</li>
  <ul>
    <li>Nova Função simular_hidrometro (O Corpo da Thread)</li>
    <li>O novo main() assume o papel de orquestrador, em vez de simulador.</li> </ul>
</ul> 