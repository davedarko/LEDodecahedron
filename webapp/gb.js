

function supports_canvas() 
{
	return !!document.createElement('canvas').getContext;
}

if (supports_canvas()) 
{
	console.log("Canvas is supported");
}
else 
{
	console.log("Canvas not supported");
}

var fileInput = document.getElementById('file-selector');
fileInput.onchange = function() 
{
	var file = fileInput.files[0];
	// $('#upload_img').hide();
	// MegaPixImage constructor accepts File/Blob object.
	var mpImg = new MegaPixImage(file);
	var orientation = 1;

	var reader2 = new FileReader ();
	reader2.onloadend = function(e) 
	{
		var exif = EXIF.readFromBinaryFile(new BinaryFile(e.target.result));
		orientation = exif.Orientation;

		var resCanvas = document.getElementById('canvas');
		mpImg.render(resCanvas, { maxWidth: 160, orientation: orientation });
		mpImg.onrender = function (target) 
		{
			url = resCanvas.toDataURL("image/jpeg");
			var tmpCtx = resCanvas.getContext('2d');
			var imgd = tmpCtx.getImageData(0, 0, 1, 1);
			// var pix = imgd.data;

			console.log(
				imgd.data,
				resCanvas.width, 
				resCanvas.height, 
				tmpCtx
			);

			// Get the CanvasPixelArray from the given coordinates and dimensions.
			var x = 0;
			var y = 0;

			var width = resCanvas.width;
			var height = resCanvas.height;

			var imgd = tmpCtx.getImageData(x, y, width, height);
			tmpCtx.imageSmoothingEnabled= false;
			var pix = imgd.data;

			// greyscaling
			for (var i = 0, n = pix.length; i < n; i += 4) 
			{
				var oldpixel = (pix[i]+pix[i+1]+pix[i+2]) / 3;
				pix[i  ] = oldpixel; // red
			    pix[i+1] = oldpixel; // green
			    pix[i+2] = oldpixel; // blue
			}

			// Loop over each pixel and invert the color.
			for (var i = 0, n = pix.length; i < n; i += 4) 
			{
				var oldpixel = pix[i];
				var newpixel = find_closest_palette_color(oldpixel);
				var quant_error = oldpixel - newpixel;

				pix[i  ] = newpixel; // red
			    pix[i+1] = newpixel; // green
			    pix[i+2] = newpixel; // blue
			    
			    if (
			    	i+4 <= pix.length && // out of pixels
			    	parseInt(i/width) == parseInt((i+4)/width) // out of line
		    	) {
			    	pix[i+4] += quant_error * 7 / 16;
			    }

			    if (
			    	(i-4 > 0) &&
			    	(i-4+width <= pix.length)
		    	) {
			    	pix[i+width-4] += parseInt(quant_error * 3 / 16);
			    }

			    if (
			    	(i+width) <= pix.length
		    	) {
			    	pix[i+width] += parseInt(quant_error * 5 / 16);
			    }

			    if (
			    	(i+4+width) <= pix.length
		    	) {
			    	pix[i+4+width] += parseInt(quant_error * 1 / 16);
			    }
			    

				// console.log(ix, iy);
				// oldpixel        = pixel[x][y];
				// newpixel        = find_closest_palette_color (oldpixel);
				// pixel[x][y]     = newpixel;
				// quant_error     = oldpixel - newpixel;
				// pixel[x+1][y  ] = pixel[x+1][y  ] + quant_error * 7 / 16;
				// pixel[x-1][y+1] = pixel[x-1][y+1] + quant_error * 3 / 16;
				// pixel[x  ][y+1] = pixel[x  ][y+1] + quant_error * 5 / 16;
				// pixel[x+1][y+1] = pixel[x+1][y+1] + quant_error * 1 / 16;

			    // pix[i  ] = pix[i]; // red
			    // pix[i+1] = pix[i+1]; // green
			    // pix[i+2] = pix[i+2]; // blue
			    // pix[i+3] = 255; // alpha
			    // i+3 is alpha (the fourth element)
			}

			// Draw the ImageData at the given (x,y) coordinates.
			tmpCtx.imageSmoothingEnabled= false;
			tmpCtx.putImageData(imgd, x, y);
			tmpCtx.imageSmoothingEnabled= false;

			var reader = new FileReader ();
			reader.onload = function (e) {
				console.log('ready!');
				// var data = new FormData();
				// data.append ("action", "upload");
				// data.append ("ajax-uploadimage", encodeURIComponent(url));

				// $.ajax ({
				//     timeout: 120000,
				//     type : "POST", 
				//     async : false,
				//     contentType : 'multipart/form-data',
				//     url : "../files_for_admin_and_browser/imgSelect/inc/imgSelect.php",
				//     data : data,
				//     cache: false,
				//     contentType: false,
				//     processData: false,
				//     success: function (response) {
				//         var jsn = $.parseJSON (response);
				//         console.log (jsn);
				//         document.getElementById('input_picture').value = jsn.data;
				//         $('#lbl_uploaded_img').val('wird hochgeladen.');
				//         $('#uploaded_img').attr ("src", url);
				//         $('#uploaded_img').load(function() {
				//             $('#uploaded_img').show();
				//         }); 
				  
				//     }
				// });
			};
			reader.readAsDataURL (file);
		};
	};
	reader2.readAsBinaryString (file);
};
