const initWasm = () => {
  return (
    new Promise((resolve, _reject) => {
      Module.onRuntimeInitialized = async _ => {
        const api = {
          addNumber: Module['add_number'],
          getMathExpression: Module['get_math_expression'],
        };

        resolve({
          addNumber: api.addNumber,
          getMathExpression: api.getMathExpression,
        });
      }
    })
  )
}

const main = async () => {
  const api = await initWasm();

  // get calculator button node
  const numberButtons = document.getElementsByClassName('calculator__number');

  // get calculator screen current text
  const calcScreenText = document.getElementById('calculator__screen-current-text');

  const onClick = (e) => {
    const numberContent = e.target.textContent.replace(/[\n\r]+|[\s]{2,}/g, ' ').trim()
    api.addNumber(numberContent);
    const mathExpression = api.getMathExpression();
    calcScreenText.innerText = mathExpression;
  };

  for (let i = 0; i < numberButtons.length; i += 1) {
    numberButtons[i].addEventListener('click', onClick)
  }
};

main();
