<?php $sendBuffer = TRUE; ob_start(); ?>
<html>
<head>
<title>IPWorks MQ 2022 Demos - AMQP</title>
<link rel="stylesheet" type="text/css" href="stylesheet.css">
<meta name="description" content="IPWorks MQ 2022 Demos - AMQP">
</head>

<body>

<div id="content">
<h1>IPWorks MQ - Demo Pages</h1>
<h2>AMQP</h2>
<p>Shows how to use the AMQP component to send and receive messages.</p>
<a href="default.php">[Other Demos]</a>
<hr/>

<?php
require_once('../include/ipworksmq_amqp.php');
require_once('../include/ipworksmq_const.php');

?>

<?php
	class MyAMQP extends IPWorksMQ_AMQP
	{
		function fireSSLServerAuthentication($param) {
			$param['accept'] = true;
			return $param;
		}
	}

	$amqp = new MyAMQP();
?>

<form method="post">
	<table>
	<tr><td>Host</td><td><input type="text" name="host" value="<?php echo isset($_POST["host"])?$_POST["host"]:""; ?>" size=35></td></tr>
	<tr><td>Port</td><td><input type="text" name="port" value="<?php echo isset($_POST["port"])?$_POST["port"]:"5672"; ?>" size=35></td></tr>
	<tr><td>User</td><td><input type="text" name="user" value="<?php echo isset($_POST["user"])?$_POST["user"]:""; ?>" size=35></td></tr>
	<tr><td>Password</td><td><input type="password" name="password" value="<?php echo isset($_POST["password"])?$_POST["password"]:""; ?>" size=35></td></tr>
	<tr><td>Enable SSL</td><td><input type="checkbox" name="ssl" value="<?php echo isset($_POST["ssl"])?$_POST["ssl"]:"1"; ?>" size=35></td></tr>
	<tr><td>Message</td><td><input type="text" name="message" value="<?php echo isset($_POST["message"])?$_POST["message"]:"Hello, world!"; ?>" size=35><input type="submit" name="send" value="Send Message"></td></tr>
	<tr><td><input type="submit" name="fetch" value="Fetch Message"></td><td></tr>
	</table>
	</form>

<?php

if ($_SERVER['REQUEST_METHOD'] == "POST") {
	try {
		//$amqp = new IPWorksMQ_AMQP();
		$amqp->setUser($_POST["user"]); //set user and password from form
		$amqp->setPassword($_POST["password"]);
		$amqp->setContainerId("ContainerId"); //set unique container id
		$amqp->setSSLEnabled(isset($_POST['ssl']) && $_POST['ssl']); //set ssl enabled or disabled according to check box
		$amqp->doConnectTo($_POST["host"],$_POST["port"]); //connect with host and port provided in form
		$amqp->doCreateSession("SessionName"); //create session, senderlink and receiverlink
		$amqp->doCreateSenderLink("SessionName","SenderLinkName","TargetName");
		$amqp->setReceiveMode(1); //0 for automatic, 1 for fetch
		$amqp->doCreateReceiverLink("SessionName","ReceiverLinkName","TargetName");
		
		if(isset($_POST['send'])) {
			//send message
			$amqp->doResetMessage();
			$amqp->setMessageValueType(17); //string type
			$amqp->setMessageValue($_POST["message"]);
			$amqp->doSendMessage("SenderLinkName");
			echo "Message sent.";
		} else if(isset($_POST['fetch'])) {
			//fetch message
			$amqp->setFetchTimeout(5); //5 second timeout
			echo "Fetching Message... ";
			$amqp->doFetchMessage("ReceiverLinkName");
			echo "Message received: ",$amqp->getReceivedMessageValue();
		}
	} catch (Exception $e) {
		echo 'Error: ', $e->getMessage(), "\n";
	}
}
?>

<br/>
<br/>
<br/>
<hr/>
NOTE: These pages are simple demos, and by no means complete applications.  They
are intended to illustrate the usage of the IPWorks MQ objects in a simple,
straightforward way.  What we are hoping to demonstrate is how simple it is to
program with our components.  If you want to know more about them, or if you have
questions, please visit <a href="http://www.nsoftware.com/?demopg-ITPHA" target="_blank">www.nsoftware.com</a> or
contact our technical <a href="http://www.nsoftware.com/support/">support</a>.
<br/>
<br/>
Copyright (c) 2023 /n software inc.
<br/>
<br/>
</div>

<div id="footer">
<center>
IPWorks MQ 2022 - Copyright (c) 2023 /n software inc. - For more information, please visit our website at <a href="http://www.nsoftware.com/?demopg-ITPHA" target="_blank">www.nsoftware.com</a>.
</center>
</div>

</body>
</html>

<?php if ($sendBuffer) ob_end_flush(); else ob_end_clean(); ?>
