satoyama-node
=============

Satoyama Sensor Network node stack

# Installation

1. Attach a sonnar, a temperature sensor and a humidity sensor. The pin numbers are specified at satoyama-chibi-lib.
2. import satoyama-chibi-lib into your Arduino IDE
3. set the short address of this collector node by hand using test-saboten/test_saboten.ino
4. load collector/collector.ino into your Saboten board. (Include all the necessary libs from third-party-libs folder)