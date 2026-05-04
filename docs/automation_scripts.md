# BlackCapy Automation Scripts

The BlackCapy firmware includes a simple script execution engine that allows running automation scripts stored on the microSD card.

## Script Location

Scripts must be placed in the `/scripts` directory on the microSD card.

## Script Format

Scripts are plain text files with one command per line. Lines starting with `#` are treated as comments and ignored.

### Supported Commands

#### delay <milliseconds>
Pause execution for the specified number of milliseconds.
```
delay 1000
```

#### print <text>
Print text to the serial console.
```
print Hello World
```

#### log <text>
Write text to the system log.
```
log Script started
```

#### gpio set <pin> <high|low|1|0>
Set a GPIO pin to high or low state.
```
gpio set 5 high
gpio set 12 low
```

#### gpio read <pin>
Read the state of a GPIO pin and print it.
```
gpio read 13
```

#### uart write <port> <data>
Write data to a UART port (currently only port 0 - Serial is supported).
```
uart write 0 Hello from script
```

## Example Script

```
# LED Blinker Script
print Starting LED demo

# Turn LED on
gpio set 5 high
print LED is ON
delay 1000

# Turn LED off
gpio set 5 low
print LED is OFF
delay 1000

print Demo completed
```

## Running Scripts

Scripts can be executed from the serial console or menu:

### From Serial Console
```
script blink_led.txt
```

### From Menu
```
script blink_led.txt
```

## Error Handling

- If a script file is not found, an error message is displayed
- Unknown commands are logged and ignored
- Known commands with invalid arguments stop script execution
- GPIO operations require valid numeric pin values
- All delays are in milliseconds
- `delay` values must be greater than zero

## Integration

The script engine integrates with the existing BlackCapy architecture:
- Uses StoragePolicy for path management
- Leverages the filesystem abstraction layer
- Uses Logger for script warnings and errors
- Commands are intentionally small and explicit

Validate bundled scripts with:

```bash
scripts/validate_assets.py
```
