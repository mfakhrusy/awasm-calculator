const initWasm = () => {
  return (
    new Promise((resolve, _reject) => {
      Module.onRuntimeInitialized = async _ => {
        const api = {
          addNumber: Module['add_number'],
          addOperator: Module['add_operator'],
          removeLastCharacter: Module['remove_last_character'],
          clearMathExpression: Module['clear_math_expression'],
          getMathExpression: Module['get_math_expression'],
          getMathResult: Module['get_math_result'],
        };

        resolve({
          addNumber: api.addNumber,
          addOperator: api.addOperator,
          removeLastCharacter: api.removeLastCharacter,
          clearMathExpression: api.clearMathExpression,
          getMathExpression: api.getMathExpression,
          getMathResult: api.getMathResult,
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
    const expr = api.getMathExpression();
    // change / to ÷ with unicode
    const innerText = expr.replaceAll('/', '\u00F7')

    calcScreenText.innerText = innerText;
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
  const onClickOperatorButton = (type) => {
    switch (type) {
      case 'division':
        api.addOperator('/');
        break;
      case 'multiplication':
        api.addOperator('x');
        break;
      case 'addition':
        api.addOperator('+');
        break;
      case 'subtraction':
        api.addOperator('-');
        break;
      default:
        break;
    }

    setExpression()
  };

  divisionNode.addEventListener('click', () => onClickOperatorButton('division'));
  multiplicationNode.addEventListener('click', () => onClickOperatorButton('multiplication'));
  additionNode.addEventListener('click', () => onClickOperatorButton('addition'));
  subtractionNode.addEventListener('click', () => onClickOperatorButton('subtraction'));

  // AC function (clear screen)
  const onClickClearButton = () => {
    api.clearMathExpression();

    setExpression();
  }

  const clearNode = document.getElementById('calculator__ac');
  clearNode.addEventListener('click', onClickClearButton);


  // delete function

  const onClickDelete = () => {
    api.removeLastCharacter();

    setExpression();
  };

  const delNode = document.getElementById('calculator__del');
  delNode.addEventListener('click', onClickDelete);

  const onClickEqual = () => {
    const result = api.getMathResult();

    console.log(result)
  };

  const equalNode = document.getElementById('calculator__equal');
  equalNode.addEventListener('click', onClickEqual);

};

main();
