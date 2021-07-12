const initWasm = () => {
  return (
    new Promise((resolve, _reject) => {
      Module.onRuntimeInitialized = async _ => {
        const api = {
          plusOne: Module.cwrap('plusOne', 'number', ['number'])
        };

        resolve({
          plusOne: api.plusOne,
        });
      }
    })
  )
}

const main = async () => {
  const api = await initWasm();

  console.log(api.plusOne(4))
};

main();
