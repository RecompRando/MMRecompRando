import socket
import json

oauth = "q02mbfu9o7r9s7mr2ralt4tiah1amb"

# Set up communications with Twitch's API
twitch = socket.socket()
twitch.connect(('irc.chat.twitch.tv', 6667))
twitch.send(f"PASS oauth:{oauth}\n".encode('utf-8')) # OAuth
twitch.send(f"NICK announcerpal\n".encode('utf-8')) # Username
twitch.send(f"JOIN #majinphil\n".encode('utf-8')) # Livestream's Channel Name

twitch.send('CAP REQ :twitch.tv/tags\n'.encode('utf-8')) # Get user badges in responses

while not (AttributeError, ConnectionResetError): # Just in case the connection has any issues from previous connections(?)
    resp = twitch.recv(2048).decode('utf-8') # Check for a successful connection
print(f"Twitch: Connected to channel")

twitch.setblocking(False)

# Constantly check for new chat messages
def message():
    try:
        resp = twitch.recv(2048).decode('utf-8')
    except BlockingIOError:
        return 0
    if resp.startswith('PING'): # Maintains connection with Twitch
        twitch.send('PONG\n'.encode('utf-8'))
    elif resp.startswith(":tmi.twitch.tv"): # Ignore other data sent by Twitch
        pass
    elif resp != "" and "PRIVMSG" in resp:
        # Filter out someone's display name and message (dumb way of doing it, but it works)
        message = resp[resp.find(" :", resp.find("PRIVMSG")) + 2 : -2]
        if "ScarecrowJam" in message:
            return 1
        elif "FROG" in message:
            return 2
        elif "CDown" in message:
            return 3
        elif "CLeft" in message:
            return 4
        elif "CRight" in message:
            return 5
        elif "CUp" in message:
            return 6
        elif "AButt" in message:
            return 7
        elif "glorp" in message:
            return 8
        elif "majinD" in message:
            return 9
        elif "rocketmess" in message:
            return 10
    return 0