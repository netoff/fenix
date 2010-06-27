function sameHeight ()
{
	var left1 = $("#left-col div.window"), left2 = $("#left-col div.text p"), right = $("#right-col div.window"), a;

	a = left1.height() + left2.height() + 50;

	if(a > right.height())
	{
		right.height(a);
	}
	else
	{
		left2.height(left2.height() + right.height() - a);
	}
}

function errorLabel(error, elem)
{
	elem.wrap($("<span style='position:relative;' />"));
	error.insertBefore(elem);
	error.css({top: - error.height() - 6});
}


$(function ()
{
	$("#forgot-password-link").click(function(){
			$("#login-form").hide();
			$("#forgot-password-form").fadeIn();
			
			sameHeight();
			
			return false;
	});
	
	$("div.alert-window input[type='submit']").click(function(){
		$("div.alert-window").hide();	
		
		return false;
	});
	
	
	
	$("#login-form").validate({errorPlacement: errorLabel});
	$("#signup-form").validate({errorPlacement: errorLabel});
	$("#forgot-password-form").validate({errorPlacement: errorLabel});

	sameHeight();
});
