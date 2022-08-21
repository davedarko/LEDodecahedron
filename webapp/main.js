var canvas = document.getElementById("canvas");
var ctx = canvas.getContext("2d");

var fileContent;
var LEDradius = 1;
var offset_x = -50;
var offset_y = 100;
var factor = 5;

ctx.strokeStyle = "green";

const content = document.querySelector('.content');
const fileSelector = document.getElementById('file-selector');

  fileSelector.addEventListener('change', (event) => {
    const fileList = event.target.files;
    file=fileList[0];

    const reader = new FileReader();
    
	  reader.addEventListener("load", () => {
	    // this will then display a text file

	    fileContent = reader.result;
	    // content.innerText = fileContent;
	    

	    var lines = fileContent.split(/\r?\n/);

	    for (i=1; i<lines.length; i++)
	    {

	    	rows = lines[i].split(';');
	    	
	    	var LEDx = rows[3] - LEDradius;
	    	var LEDy = rows[4] - LEDradius;
	    	
	    	ctx.strokeRect(
	    		factor*(LEDx + offset_x),
	    		factor*(LEDy + offset_y),
	    		factor*LEDradius * 2,
	    		factor*LEDradius * 2
    		);
	    	console.log(factor*LEDx + offset_x,
	    		factor*LEDy + offset_y,
	    		factor*LEDradius * 2,
	    		factor*LEDradius * 2);
	    }


	  }, false);

	  if (file) {
	    reader.readAsText(file);
	  }
  });