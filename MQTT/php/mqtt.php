<?php $sendBuffer = TRUE; ob_start(); ?>
<html>
<head>
<title>IPWorks MQ 2022 Demos - MQTT</title>
<link rel="stylesheet" type="text/css" href="stylesheet.css">
<meta name="description" content="IPWorks MQ 2022 Demos - MQTT">
</head>

<body>

<div id="content">
<h1>IPWorks MQ - Demo Pages</h1>
<h2>MQTT</h2>
<p>Uses the MQTT demo to send and receive messages over SSL and plaintext.</p>
<a href="default.php">[Other Demos]</a>
<hr/>

<?php
require_once('../include/ipworksmq_mqtt.php');
require_once('../include/ipworksmq_const.php');

?>

<?php
	global $receivedMessage;
	$receivedMessage = null;
	class MyMQTT extends IPWorksMQ_MQTT
	{
		// Fires when a message is received
		function fireMessageIn($param) {
			global $receivedMessage;
			$receivedMessage = $param;
		}
	}

	$mqtt = new MyMQTT();

	// Get connection variables from form or use defaults
	$host= (empty($_POST['host'])) ? 'test.mosquitto.org' : $_POST['host'];
	$port= (empty($_POST['port'])) ? '1883' : $_POST['port'];
	$topic= (empty($_POST['topic'])) ? 'nsoftware/test2' : $_POST['topic'];
	$qos= (empty($_POST['qos'])) ? '0' : $_POST['qos'];
	$message = (empty($_POST['message'])) ? 'Hello World!' : $_POST['message'];
?>

	<form method="post">
	<table>
	<tr><td>Host</td><td><input type="text" name="host" value="<?php echo $host; ?>" size="35"></td></tr>
	<tr><td>Port</td><td><input type="text" name="port" value="<?php echo $port; ?>" size="35"></td></tr>
	<tr><td>Topic</td><td><input type="text" name="topic" value="<?php echo $topic; ?>" size="35"></td></tr>
	<tr><td>QoS</td><td><input type="text" name="qos" value="<?php echo $qos; ?>" size="35"></td></tr>
	<tr><td>Message</td><td><input type="text" name="message" value="<?php echo $message; ?>" size="35"></td></tr>
	<tr><td></td><td><input type="submit" value="Publish Message"></td></tr>
	</table>
	</form>

<?php

if ($_SERVER['REQUEST_METHOD'] == "POST") {

	try{
		// Connect, subscribe to the topic, and publish the message
		$mqtt->doConnectTo($host, $port);
		$mqtt->doSubscribe($topic, $qos);
		$mqtt->doPublishMessage($topic, $qos, $message);

		// Wait for a message to be received
		global $receivedMessage;
		while($receivedMessage == null)
		{
			$mqtt->doEvents();
		}
		
		// Display the received message's details
		echo "<hr><b>Received from " . $receivedMessage['topic'];
		echo " at QoS " . $receivedMessage['qos'];
		if ($receivedMessage['retained']) {
		echo " [Retained message]";
		}
		echo ": ";
		echo $receivedMessage['message'] . "</b>";
		$mqtt->doDisconnect();
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
