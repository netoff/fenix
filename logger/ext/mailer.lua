local MAIL_QUEUE_TABLE = 'mail.queue'
local MAIL_SENDER = 'kliknik.com'
local MAIL_FROM = 'noreplay@kliknik.com'

local ADMIN_ADDRESS = "dushan01@kliknik.com"

local REGISTRATION_MAIL_TEXT = [[
Hello,

Thank you for a sign up with kliknik. 

Right now we are in a limited beta. As soon as more slots become available
we will send you activation link, so you can verify you mail address and start 
using our service.

In a meantime if you have any questions, please feel free to email us at dev@kliknik.com.

http://www.kliknik.com
]]

local ACTIVATION_MAIL_TEXT = [[
Hello,

Your account at kliknik is activated. You can start using our analytics,
by installing JavaScript code. Please paste the code to end of you website template,
just before closing '</body>' tag. Code has to be included in every page you want to track.

//Code:

<script src='http://cdn.kliknik.com'></script>
<script type='text/javascript'>
_fnx_get_tracker('%s');
</script>

//End Code

Thank you,
http://www.kliknik.com
]]
