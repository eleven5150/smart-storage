// $( document ).ready(function() { //checking for press button
// 	$("#btn2").click(
// 		function(){
// 			//document.write('kek');
// 			sendAjaxForm('result_form', 'send2', 'send.php'); //start send function
// 		    return false; 
// 		}
// 	);
// });

function sendAjaxForm(result_form, ajax_form, url) { // send function
    jQuery.ajax({
        url:      url, //url страницы (send.php)
        type:     "POST", //метод отправки
        dataType: "html", //формат данных
        data: jQuery("#"+ajax_form).serialize(),  // Сеарилизуем объект
        success: function(response) { //Данные отправлены успешно
        	//document.write(response); //command for debug
        	result = jQuery.parseJSON(response);
        	document.getElementById(result_form).innerHTML = result.answer; // respond from send.php
    	},
    	error: function(response) { // Данные не отправлены
    		document.getElementById(result_form).innerHTML = "Ошибка. Данные не отправлены."; //error
    	}
 	});
}
