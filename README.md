This is a linux module to remove the throttling of some PC brands processors.
Inspired by ThrottleStop on windows. You first have to insert the linux backend module
 
https://github.com/PedroHLeal/linuxthrcontrol

Building
---

In the directory of the project enter

	$make

Commands:
---

Displaying current information of the processor state

	#thrctl info

Shutting throttle control off

	#thrctl off

Turning throttle control on
	
	#thrctl on

Setting max frequency in MHz

	#thrctl max-freq <max-freq(MHz)>
