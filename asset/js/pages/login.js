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

$(function ()
{
	$("#signup-thanks input[type='submit']").click(function(){
		$("#signup-thanks").hide();	
		
		return false;
	});
	
	$("#login-form").validate();
	$("#signup-form").validate();

	sameHeight();
});
