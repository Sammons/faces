var faces = require('./build/Release/eigenface.node');

faces.train();
console.log(faces.evaluate('./croppedfaces/yaleB01_P00A+000E+00.pgm'));