function sameHeight (sections)
{
	var max, i, section;
	
	max = 0;

	for (i = 0; i < sections.length; i++)
	{
		//max = 0;
		
		section = sections[i];
		
		if ($(section).outerHeight() > max)
		{
			max = $(section).outerHeight();
		}		
	}
	
	for (i = 0; i < sections.length; i++)
	{
		$(sections[i]).height(max);
	}
	
	return max;
}

$(function ()
{
	$("#signup-thanks input[type='submit']").click(function(){
		$("#signup-thanks").hide();	
		
		return false;
	});
	
	$("#login-form").validate();
	$("#signup-form").validate();

	sameHeight($("div.same-height"));	
});
