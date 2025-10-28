# Semáforo offline pausável (Arduino) 

Este projeto implementa um semáforo com três LEDs (vermelho, amarelo e verde) e um botão para pausar/retomar o ciclo. O comportamento segue o padrão:
- 6 segundos no vermelho
- 4 segundos no verde
- 2 segundos no amarelo

Referências do projeto:
- Vídeo (execução no modelo físico): https://youtu.be/ml_ekHULPiw
- Simulação (Tinkercad): https://www.tinkercad.com/things/8Zy5om27QAq-semaforo-pausavel?sharecode=ZJF7l093WR7b8v8Ha5xddv1xVmENgTfdlMOBuhigGUo


## Materiais e especificações

| Componente               | Quantidade | Especificação/Observações                       |
|--------------------------|------------|-------------------------------------------------|
| Arduino Uno (ou compatível) | 1          | Alimentação via USB                             |
| Protoboard               | 1          | 400 ou 830 pontos                               |
| LED Vermelho             | 1          | 5 mm (típico)                                   |
| LED Amarelo              | 1          | 5 mm (típico)                                   |
| LED Verde                | 1          | 5 mm (típico)                                   |
| Resistores para LEDs     | 3          | 220 Ω a 330 Ω (um por LED, em série)            |
| Botão (push button)      | 1          | Usado com INPUT_PULLUP (sem resistor externo)   |
| Jumpers                  | —          | Macho–macho                                     |

Observação: usando `INPUT_PULLUP` no pino do botão, não é necessário resistor externo; conecte o botão entre o pino e o GND. O nível lógico fica invertido (pressionado = LOW).


## Esquema elétrico (descrição)

Mapeamento de pinos (conforme o código `codigo.ino`):
- Verde → pino digital 8
- Amarelo → pino digital 9
- Vermelho → pino digital 10
- Botão → pino digital 2 (configurado como `INPUT_PULLUP`), outro terminal do botão no GND

Ligação dos LEDs (cada um):
1. Anodo (perna maior) do LED → resistor de 330 Ω → pino digital correspondente (8, 9 ou 10)
2. Catodo (perna menor) do LED → GND

Botão (pause):
- Um terminal no pino 2
- Outro terminal no GND
- Sem resistor externo (puxador interno ativado com `INPUT_PULLUP`)


## Passo a passo de montagem

1. Posicione os três LEDs na protoboard, separados, e identifique as cores.
2. Conecte um resistor (330 Ω) do anodo de cada LED ao respectivo pino do Arduino: 8 (verde), 9 (amarelo), 10 (vermelho).
3. Conecte os catodos dos LEDs à barra de GND da protoboard e ligue essa barra ao GND do Arduino.
4. Instale o botão. Conecte um terminal ao pino 2 e o outro ao GND.
5. Garanta que há um fio do GND do Arduino para a barra negativa (GND) da protoboard.
6. Revise as conexões antes de energizar o Arduino.

Sugestão: mantenha fios organizados por cor (ex.: verde para o LED verde, amarelo, vermelho; preto para GND) para facilitar a visualização e depuração.


## Programação e lógica do semáforo

O ciclo esperado é contínuo com as seguintes temporizações:
- Vermelho: 6 s
- Verde: 4 s
- Amarelo: 2 s

O botão no pino 2 pausa o ciclo: ao pressionar, o semáforo mantém a cor atual acesa e não avança até soltar o botão.

O código do projeto está em `codigo.ino` (usado tanto no Arduino físico quanto na simulação do Tinkercad).


## Explicação do código (por partes)

Arquivo: `codigo.ino`

### 1) Classe `Semaforo`

```
class Semaforo {
	const char* availableColors[3] = { "GREEN", "YELLOW", "RED" };
	const int availablePins[3] = { 8, 9, 10 };
	int currentColorIndex = 0;
	...
};
```

- `availableColors` e `availablePins` definem o mapeamento cor → pino: GREEN→8, YELLOW→9, RED→10.
- `currentColorIndex` controla a cor atual: 0=verde, 1=amarelo, 2=vermelho.

```
int getPinFromColor(const char* color) {
	int pin = -1;
	for (int i = 0; i < 3; i++) {
		if (availableColors[i] == color) {
			pin = availablePins[i];
			break;
		}
	}
	return pin;
}
```

- Função utilitária para obter o pino a partir da cor.

```
void changeColor(const bool isButtonPressed) {
	if (isButtonPressed == true) {
		return; // pausa: mantém a cor atual
	}

	digitalWrite(getPinFromColor(availableColors[currentColorIndex]), HIGH);
	currentColorIndex++;
	if (currentColorIndex >= 3) currentColorIndex = 0;
}
```

- Quando não está pausado, acende o LED da cor atual e avança o índice para a próxima cor (ciclo 0→1→2→0).

### 2) Variáveis globais

```
Semaforo semaforo;
int currentColorIndex = semaforo.getColor();
int currentDelay = 4000;
bool isButtonPressed = false;
```

- Guardam o estado atual lido da classe, o delay em ms e o estado do botão.

### 3) `setup()`

```
pinMode(semaforo.getPinFromColor("GREEN"), OUTPUT);
pinMode(semaforo.getPinFromColor("YELLOW"), OUTPUT);
pinMode(semaforo.getPinFromColor("RED"), OUTPUT);
pinMode(2, INPUT_PULLUP);
```

- Define os pinos 8/9/10 como saída (via mapeamento) e o pino 2 como entrada com pull-up interno.

### 4) `loop()`

```
currentColorIndex = semaforo.getColor();
isButtonPressed = digitalRead(2) != HIGH; // INPUT_PULLUP: pressionado = LOW

if (isButtonPressed == false) {
	// Rodando normalmente: limpa antes de acender a cor atual
	digitalWrite(semaforo.getPinFromColor("GREEN"), LOW);
	digitalWrite(semaforo.getPinFromColor("YELLOW"), LOW);
	digitalWrite(semaforo.getPinFromColor("RED"), LOW);
}

switch (currentColorIndex) {
	case 2: // vermelho
		semaforo.changeColor(isButtonPressed);
		currentDelay = 6000;
		break;
	case 1: // amarelo
		semaforo.changeColor(isButtonPressed);
		currentDelay = 2000;
		break;
	case 0: // verde (padrão)
	default:
		semaforo.changeColor(isButtonPressed);
		currentDelay = 4000;
		break;
}

delay(currentDelay);
```

- Lê o botão (com lógica invertida por `INPUT_PULLUP`: pressionado = `true`).
- Em operação normal (não pressionado), apaga todos os LEDs e, em seguida, `changeColor` acende a cor corrente.
- O tempo de `delay` reflete a cor que acabou de acender (vermelho 6000 ms, amarelo 2000 ms, verde 4000 ms).
- Quando o botão é pressionado, `changeColor` retorna sem alterar o estado: a cor atual permanece acesa (pausa).


## Como executar

### Opção A — Arduino físico
1. Monte o circuito conforme descrito acima.
2. Abra este repositório na Arduino IDE e carregue o arquivo `codigo.ino` para a placa.
3. Alimente a placa via USB. Observe o ciclo de cores (V→A→R) com os tempos 4 s, 2 s e 6 s, respectivamente.
4. Pressione o botão para pausar (a cor atual permanece acesa). Solte para retomar o ciclo.

### Opção B — Tinkercad
1. Acesse a simulação: https://www.tinkercad.com/things/8Zy5om27QAq-semaforo-pausavel?sharecode=ZJF7l093WR7b8v8Ha5xddv1xVmENgTfdlMOBuhigGUo
2. Clique em “Iniciar simulação”. Interaja com o botão virtual para pausar/retomar.
