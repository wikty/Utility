var fs = require('fs');

function trim(s) {
	return s.replace(/(^\s*)|(\s*$)/, '');
}

function stat(content) {
	var wordsCount = {};
	var count = 0;

	var lines = content.split('\r\n');
	lines.forEach(function(line) {
		var words = line.split(/\s+/);
		words.forEach(function(word) {
			word = trim(word);
			if(word) {
				if(!wordsCount.hasOwnProperty(word)) {
					wordsCount[word] = 0;
				}
				wordsCount[word]++;
				count++;
			}
		});
	});

	var sum = 0;
	for(var key in wordsCount) {
		var p = wordsCount[key]/count;

		sum -= p * Math.log2(p);
	}

	return sum;
}

var content = fs.readFileSync('./chn', 'utf8');
var chnEntropy = stat(content);
content = fs.readFileSync('./eng', 'utf8');
var engEntropy = stat(content);

console.log('Chinease Entropy is ' + chnEntropy);
console.log('English Entropy is ' + engEntropy);