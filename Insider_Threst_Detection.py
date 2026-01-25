import time
import os
import hashlib
import getpass
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from datetime import datetime

# ----------------------------
# Configuration
# ----------------------------
DECOY_FOLDER = "Decoy_Files"
LOG_FILE = "insider_threat_log.txt"

# ----------------------------
# Create Decoy Files
# ----------------------------
def create_decoy_files():
    """Creates the decoy directory and files if they don't exist."""
    os.makedirs(DECOY_FOLDER, exist_ok=True)
    files = ["salary_records.txt", "admin_passwords.txt", "confidential_plan.txt"]

    for file in files:
        path = os.path.join(DECOY_FOLDER, file)
        if not os.path.exists(path):
            with open(path, "w") as f:
                f.write("CONFIDENTIAL DATA - AUTHORIZED PERSONNEL ONLY\n")

# ----------------------------
# Hash Function (Integrity)
# ----------------------------
def calculate_hash(file_path):
    """Generates a SHA-256 hash to track file integrity."""
    sha256 = hashlib.sha256()
    try:
        with open(file_path, "rb") as f:
            sha256.update(f.read())
        return sha256.hexdigest()
    except:
        return "FILE DELETED"

# ----------------------------
# Alert System
# ----------------------------
def alert(file_path, event_type):
    """Logs and prints detailed forensic information."""
    user = getpass.getuser()
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    file_hash = calculate_hash(file_path)

    message = (
        f"[ALERT] Event: {event_type}\n"
        f"User: {user}\n"
        f"File: {file_path}\n"
        f"Hash: {file_hash}\n"
        f"Time: {timestamp}\n"
        f"{'-'*50}\n"
    )

    print("\n⚠️ INSIDER THREAT DETECTED ⚠️")
    print(message)

    with open(LOG_FILE, "a") as log:
        log.write(message)

# ----------------------------
# Monitoring Class
# ----------------------------
class DecoyMonitor(FileSystemEventHandler):
    """Handles specific file system events."""

    def on_modified(self, event):
        if not event.is_directory:
            alert(event.src_path, "MODIFIED")

    def on_deleted(self, event):
        if not event.is_directory:
            alert(event.src_path, "DELETED")

    def on_created(self, event):
        if not event.is_directory:
            alert(event.src_path, "CREATED")

# ----------------------------
# Start Monitoring
# ----------------------------
def start_monitoring():
    """Initializes the watchdog observer."""
    observer = Observer()
    event_handler = DecoyMonitor()
    observer.schedule(event_handler, DECOY_FOLDER, recursive=False)
    observer.start()

    print("🔒 Mini SOC – Insider Threat Monitoring Started")
    print(f"📁 Monitoring Decoy Folder: {DECOY_FOLDER}")
    print("Press CTRL+C to stop\n")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
        print("\n🛑 Monitoring Stopped")

    observer.join()

# ----------------------------
# Main
# ----------------------------
if __name__ == "__main__":
    print("=== Security Automation Toolkit (Mini SOC) ===\n")
    create_decoy_files()
    start_monitoring()