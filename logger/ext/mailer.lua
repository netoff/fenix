MAIL_QUEUE_TABLE = 'mail.queue'
MAIL_SENDER = 'kliknik.com'
MAIL_FROM = 'noreplay@kliknik.com'

ADMIN_ADDRESS = "dushan01@kliknik.com"

REGISTRATION_MAIL_TEXT = [[
Hello,

Thank you for a sign up with kliknik. 

Right now we are in a limited beta. As soon as more slots become available
we will send you activation link, so you can verify you mail address and start 
using our service.

In a meantime if you have any questions, please feel free to email us at dev@kliknik.com.

http://www.kliknik.com
]]

ACTIVATION_MAIL_TEXT = [[
Hello,

Your account at kliknik is activated. You can start using our analytics,
by installing JavaScript code. Please paste the code to end of you website template,
just before closing '</body>' tag. Code has to be included in every page you want to track.

//Code:

<script src='http://cdn.kliknik.com/loader.js'></script>
<script type='text/javascript'>
_fnx_get_tracker('%s');
</script>

//End Code

Thank you,
http://www.kliknik.com
]]

RESET_MAIL_TEXT = [[
Hello,

You reset your password at kliknik.com. Please click on a following link to set
your new password:

http://www.kliknik.com/analytics/login/reset?t=%s

http://www.kliknik.com
]]
