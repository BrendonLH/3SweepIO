## Reyax Lite122 → ESP32-C3

RED    = 3.3V
BLACK  = GND
GREEN  = TXD (Reyax → ESP32-C3 RX)
YELLOW = RXD (ESP32-C3 TX → Reyax)
ORANGE = NRST
WHITE  = PA7 (unused)

### Connections

VDD   -> RED    -> 3.3V
GND   -> BLACK  -> GND
TXD   -> GREEN  -> GPIO20 (RX)
RXD   -> YELLOW -> GPIO21 (TX)
NRST  -> ORANGE -> Optional GPIO
PA7   -> WHITE  -> Not Connected

"These ranging modules work great... when they work. Out of about 20 purchased so far, about 4 have been DOA. SUGGESTION: STOP SOLDERING THE 90 DEGREE HEADERS ON IN THE FACTORY! Not everyone wants that mount and since the pin rivets are tiny and delicate, unsoldering and re-soldering a different header can result in damage. Just throw the loose header in the box. CONFIGURATION TIP: If you are certain your RX/TX wiring is correct and your AT configuration commands are not returning "OK" over UART, perform a module firmware reset by momentarily taking the NRST pin to ground before sending the AT commands. I have found this to correct the problem several times." - amazon review

# Reyax Lite122 (RYUW122_V1.0.21) Quick Commands

# Reyax RYUW122 Command Cheat Sheet

## Basic Commands

### Test Communication
Checks if the module is responding.

Command:
```text
AT
```

Response:
```text
+OK
```

---

### Reset Module
Reboots the module.

Command:
```text
AT+RESET
```

Response:
```text
+RESET
+READY
```

---

### Factory Reset
Restores all settings to defaults.

Command:
```text
AT+FACTORY
```

---

## Device Mode

### Set TAG Mode
Used for mobile devices being tracked.

Command:
```text
AT+MODE=0
```

---

### Set ANCHOR Mode
Used for fixed reference points.

Command:
```text
AT+MODE=1
```

---

### Check Current Mode

Command:
```text
AT+MODE?
```

Example:
```text
+MODE=1
```

---

## Network Settings

### Set Network ID
Devices must share the same Network ID to communicate.

Command:
```text
AT+NETWORKID=REYAX123
```

---

### View Network ID

Command:
```text
AT+NETWORKID?
```

---

### Set Device Address
Every device should have a unique address.

Command:
```text
AT+ADDRESS=TAG00001
```

---

### View Device Address

Command:
```text
AT+ADDRESS?
```

---

### Set Encryption Password
Devices must use the same password.

Command:
```text
AT+CPIN=00000000000000000000000000000000
```

---

### View Encryption Password

Command:
```text
AT+CPIN?
```

---

## Radio Settings

### Set RF Channel

Channel 5 (Default):
```text
AT+CHANNEL=5
```

Channel 9:
```text
AT+CHANNEL=9
```

---

### Set Data Rate

850 Kbps:
```text
AT+BANDWIDTH=0
```

6.8 Mbps:
```text
AT+BANDWIDTH=1
```

---

### Set RF Power

Maximum Power:
```text
AT+CRFOP=5
```

Lower Power:
```text
AT+CRFOP=0
```

---

## Information

### Firmware Version

Command:
```text
AT+VER?
```

Example:
```text
+VER=RYUW122_V1.0.21
```

---

### Unique Hardware ID

Command:
```text
AT+UID?
```

---

## Distance Measurement

### Adjust Distance Calibration

Subtract 11 cm:
```text
AT+CAL=-11
```

Add 5 cm:
```text
AT+CAL=5
```

View Calibration:
```text
AT+CAL?
```

---

## Sending Data

### TAG Send Data

Command:
```text
AT+TAG_SEND=5,HELLO
```

Stores data in the TAG for an Anchor to read.

---

### ANCHOR Send Data

Command:
```text
AT+ANCHOR_SEND=TAG00001,4,TEST
```

Sends data to a TAG and receives distance information.

---

## Received Messages

### Message Received by Anchor

Example:
```text
+ANCHOR_RCV=TAG00001,5,HELLO,40 cm
```

Meaning:
- Address = TAG00001
- Data = HELLO
- Distance = 40 cm

---

### Message Received by Tag

Example:
```text
+TAG_RCV=4,TEST
```

Meaning:
- Data received = TEST

---

## RSSI (Signal Strength)

Enable RSSI:
```text
AT+RSSI=1
```

Disable RSSI:
```text
AT+RSSI=0
```

Check RSSI Setting:
```text
AT+RSSI?
```

---

## Common Setup

### Anchor Setup

```text
AT+MODE=1
AT+NETWORKID=REYAX123
AT+ADDRESS=ANCHOR01
```

### Tag Setup

```text
AT+MODE=0
AT+NETWORKID=REYAX123
AT+ADDRESS=TAG00001
```

Both devices must use:
- Same NETWORKID
- Same CPIN password
- Different ADDRESS values

---

## Error Codes

| Error | Meaning |
|---------|---------|
| +ERR=1 | Missing Enter |
| +ERR=2 | Invalid command format |
| +ERR=3 | Invalid parameter |
| +ERR=4 | Command failed |
| +ERR=5 | Unknown command |