var faces = require('./build/Release/eigenface.node');

faces.train();

console.log(faces.score('./Other.pgm'));
console.log(faces.score('./Ben1.png'));
console.log(faces.score('./Ben2.png'));

