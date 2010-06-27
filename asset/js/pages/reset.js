function errorLabel(error, elem)
{
	elem.wrap($("<span style='position:relative;' />"));
	error.insertBefore(elem);
	error.css({top: - error.height() - 6});
}
$(function ()
{
	$("#reset-form").validate({errorPlacement: errorLabel});
});
