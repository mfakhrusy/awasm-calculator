const initWasm = () => {
  return (
    new Promise((resolve, _reject) => {
      Module.onRuntimeInitialized = async _ => {
        const api = {
          addNumber: Module['add_number'],
          addOperator: Module['add_operator'],
          clearMathExpression: Module['clear_math_expression'],
          getMathExpression: Module['get_math_expression'],
        };

        resolve({
          addNumber: api.addNumber,
          addOperator: api.addOperator,
          clearMathExpression: api.clearMathExpression,
          getMathExpression: api.getMathExpression,
        });
      }
    })
  )
}

const getTextContent = str => str.replace(/[\n\r]+|[\s]{2,}/g, ' ').trim();

const main = async () => {
  const api = await initWasm();

  // get calculator button node
  const numberButtons = document.getElementsByClassName('calculator__number');

  // get calculator screen current text
  const calcScreenText = document.getElementById('calculator__screen-current-text');

  const setExpression = () => {
    calcScreenText.innerText = api.getMathExpression();
  };

  // define function for number buttons
  const onClickNumberButton = (e) => {
    const content = getTextContent(e.target.textContent)
    api.addNumber(content);

    setExpression();
  };

  for (let i = 0; i < numberButtons.length; i += 1) {
    numberButtons[i].addEventListener('click', onClickNumberButton)
  }

  const divisionNode = document.getElementById('calculator__division');
  const multiplicationNode = document.getElementById('calculator__multiplication');
  const additionNode = document.getElementById('calculator__addition');
  const subtractionNode = document.getElementById('calculator__subtraction');

  // define function for 4 operator above
  const onClickOperatorButton = (e) => {
    const content = getTextContent(e.target.textContent)
    api.addOperator(content);

    setExpression()
  };

  divisionNode.addEventListener('click', onClickOperatorButton);
  multiplicationNode.addEventListener('click', onClickOperatorButton);
  additionNode.addEventListener('click', onClickOperatorButton);
  subtractionNode.addEventListener('click', onClickOperatorButton);

  // AC function (clear screen)
  const onClickClearButton = () => {
    api.clearMathExpression();

    setExpression();
  }

  const clearNode = document.getElementById('calculator__ac');
  clearNode.addEventListener('click', onClickClearButton);

};

main();
