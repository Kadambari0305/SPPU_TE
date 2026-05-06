
print("     INTELLIGENT HELP DESK EXPERT SYSTEM")

print("Answer in yes or no\n")

network = 0
software = 0
hardware = 0
account = 0
security = 0

internet = input("Internet not working? : ")
wifi = input("WiFi connected but no internet? : ")
email = input("Unable to send/receive email? : ")
server = input("Cannot access server/shared drive? : ")
lan = input("LAN cable disconnected? : ")
dns = input("Website not loading but internet connected? : ")
vpn = input("VPN not connecting? : ")

slow = input("System very slow? : ")
error = input("Software error message? : ")
install = input("Unable to install software? : ")
crash = input("Application crashing? : ")
update = input("Software update failed? : ")
license = input("License expired message? : ")
compat = input("Software not compatible with OS? : ")

noise = input("Computer making unusual noise? : ")
heat = input("System overheating? : ")
freeze = input("System freezing? : ")
restart = input("System restarting automatically? : ")
desktop = input("Desktop not working smoothly? : ")
monitor = input("Monitor not displaying? : ")
keyboard = input("Keyboard not working? : ")
mouse = input("Mouse not working? : ")
battery = input("Laptop battery draining fast? : ")

login = input("Unable to login? : ")
password = input("Forgot password? : ")
locked = input("Account locked? : ")
access = input("Access denied to folder/file? : ")
permission = input("No permission to application? : ")

virus = input("Virus warning or popup? : ")
ads = input("Too many ads/popups? : ")
unknown = input("Unknown software installed? : ")
phishing = input("Suspicious email received? : ")
usb = input("Unknown USB detected? : ")
firewall = input("Firewall blocking application? : ")

print("\nAnalyzing problem...\n")

if internet.lower()=="yes":
    network += 3
if wifi.lower()=="yes":
    network += 2
if email.lower()=="yes":
    network += 2
if server.lower()=="yes":
    network += 3
if lan.lower()=="yes":
    network += 3
if dns.lower()=="yes":
    network += 2
if vpn.lower()=="yes":
    network += 2

if slow.lower()=="yes":
    software += 1
if error.lower()=="yes":
    software += 3
if install.lower()=="yes":
    software += 2
if crash.lower()=="yes":
    software += 3
if update.lower()=="yes":
    software += 2
if license.lower()=="yes":
    software += 2
if compat.lower()=="yes":
    software += 3

if noise.lower()=="yes":
    hardware += 3
if heat.lower()=="yes":
    hardware += 3
if freeze.lower()=="yes":
    hardware += 2
if restart.lower()=="yes":
    hardware += 3
if desktop.lower()=="yes":
    hardware += 2
if monitor.lower()=="yes":
    hardware += 3
if keyboard.lower()=="yes":
    hardware += 2
if mouse.lower()=="yes":
    hardware += 2
if battery.lower()=="yes":
    hardware += 2

if login.lower()=="yes":
    account += 3
if password.lower()=="yes":
    account += 2
if locked.lower()=="yes":
    account += 3
if access.lower()=="yes":
    account += 2
if permission.lower()=="yes":
    account += 2

if virus.lower()=="yes":
    security += 4
if ads.lower()=="yes":
    security += 2
if unknown.lower()=="yes":
    security += 3
if phishing.lower()=="yes":
    security += 3
if usb.lower()=="yes":
    security += 3
if firewall.lower()=="yes":
    security += 2


scores = {
    "Network Issue": network,
    "Software Issue": software,
    "Hardware Issue": hardware,
    "Account Issue": account,
    "Security Threat": security
}

issue = max(scores, key=scores.get)
max_score = scores[issue]

if max_score >= 7:
    severity = "HIGH"
elif max_score >= 4:
    severity = "MEDIUM"
else:
    severity = "LOW"


print("           EXPERT SYSTEM RESULT")


print("Detected Issue :", issue)
print("Severity Level :", severity)
print("Score :", max_score)

if issue == "Network Issue":
    print("Action: Check router, LAN cable, DNS or contact network team")

elif issue == "Software Issue":
    print("Action: Reinstall / update application")

elif issue == "Hardware Issue":
    print("Action: Contact hardware technician")

elif issue == "Account Issue":
    print("Action: Reset password / check access permissions")

elif issue == "Security Threat":
    print("Action: Run antivirus & disconnect internet immediately")

print("\nThank you for using Help Desk Expert System")