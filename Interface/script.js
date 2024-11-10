let porta;
let leitor;
let fluxoEntrada;
let bufferLeitura = ""; // Buffer para armazenar dados recebidos
let modoManual = false; // Variável para controlar o modo manual/automático
let estaLendo = false; // Controle se está lendo dados ou não

const elementoStatus = document.getElementById("status");
const elementoErro = document.getElementById("erro");
const elementoHorizontal = document.getElementById("horizontal");
const elementoVertical = document.getElementById("vertical");
const botaoConectar = document.getElementById("botaoConectar");
const botaoDefinirAngulos = document.getElementById("botaoDefinirAngulos");
const botaoModoAutomatico = document.getElementById("botaoModoAutomatico");
const horizontalManual = document.getElementById("horizontalManual");
const verticalManual = document.getElementById("verticalManual");

// Função que é chamada ao clicar no botão Conectar
botaoConectar.addEventListener("click", async () => {
  try {
    // Solicita permissão para o uso da porta serial
    porta = await navigator.serial.requestPort();
    await porta.open({ baudRate: 9600 });

    elementoStatus.textContent = "Conectado ao Arduino";
    elementoErro.textContent = "";
    botaoConectar.style.display = "none"; // Esconde o botão após conexão

    // Lê os dados serial
    fluxoEntrada = porta.readable.getReader();
    estaLendo = true; // Marca que a leitura está em andamento
    loopLeitura();
  } catch (erro) {
    elementoErro.textContent = "Erro ao conectar: " + erro;
  }
});

// Alterna para o modo manual, desativa a leitura automática
botaoDefinirAngulos.addEventListener("click", async () => {
  modoManual = true;
  estaLendo = false; // Pausa a leitura durante o envio de dados manuais

  try {
    const horizontal = horizontalManual.value;
    const vertical = verticalManual.value;

    // Envia os ângulos manuais para o Arduino
    const dados = `${horizontal},${vertical}\n`;
    const codificador = new TextEncoder();

    // Cria um novo escritor para o envio
    const escritor = porta.writable.getWriter();
    await escritor.write(codificador.encode(dados));

    elementoErro.textContent = ""; // Limpa a mensagem de erro
    elementoStatus.textContent = "Ângulos manuais enviados!";

    // Libera o escritor após o envio
    await escritor.releaseLock();

    // Atualiza os campos Ângulo Horizontal e Ângulo Vertical com os valores enviados
    elementoHorizontal.textContent = horizontal;
    elementoVertical.textContent = vertical;

    // Retoma a leitura serial após o envio
    estaLendo = true;
    loopLeitura();
  } catch (erro) {
    elementoErro.textContent = "Erro ao enviar ângulos: " + erro;
    estaLendo = true; // Garantir que a leitura retorne em caso de erro
    loopLeitura();
  }
});

// Volta para o modo automático
botaoModoAutomatico.addEventListener("click", () => {
  modoManual = false;
  elementoStatus.textContent = "Modo Automático Ativado";
  elementoErro.textContent = ""; // Limpa qualquer erro anterior
});

// Função de leitura contínua
async function loopLeitura() {
  if (!estaLendo) return; // Não lê dados se o modo manual estiver ativo

  try {
    while (true) {
      const { value, done } = await fluxoEntrada.read();
      if (done) {
        console.log("Fluxo de entrada fechado.");
        break;
      }
      const texto = new TextDecoder().decode(value);

      // Acumula os dados recebidos
      bufferLeitura += texto;

      // Processa os dados recebidos quando encontra uma nova linha
      let linhas = bufferLeitura.split("\n");

      // Processa todas as linhas exceto a última (que pode ser uma linha incompleta)
      for (let i = 0; i < linhas.length - 1; i++) {
        processaDadosArduino(linhas[i].trim());
      }

      // Mantém qualquer sobra de dados para a próxima leitura
      bufferLeitura = linhas[linhas.length - 1];
    }
  } catch (erro) {
    console.error("Erro na leitura dos dados: ", erro);
    elementoErro.textContent = "Erro ao ler dados do Arduino.";
  }
}

// Função que processa os dados recebidos do Arduino
function processaDadosArduino(dados) {
  if (modoManual) {
    return; // Não processa os dados no modo manual
  }

  const [horizontal, vertical] = dados.split(",").map(val => val.trim());
  if (horizontal && vertical) {
    if (!isNaN(horizontal) && !isNaN(vertical)) {
      elementoHorizontal.textContent = horizontal;
      elementoVertical.textContent = vertical;
      elementoErro.textContent = ""; // Limpa a mensagem de erro se os dados forem válidos
    } else {
      elementoErro.textContent = "Dados inválidos recebidos do Arduino.";
    }
  } else {
    elementoErro.textContent = "Dados incompletos recebidos do Arduino.";
  }
}