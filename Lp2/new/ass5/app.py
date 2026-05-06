from flask import Flask, render_template, request, jsonify
import random

app = Flask(__name__)

# 100+ Q&A
qa = {
    # Appointment
    "appointment":"Book via website or call reception.",
    "book appointment":"Use online portal.",
    "cancel appointment":"Call hospital.",
    "reschedule":"You can reschedule via phone.",
    "timing":"9 AM to 8 PM.",
    "reception number": "1800-120-1965",
    "consultation fees": "Consultation fees vary by doctor and specialty, usually ranging from ₹300 to ₹1000.",
    

    # Doctors
    "doctor":"We have specialist doctors.",
    "cardiologist":"Heart specialist available.",
    "dentist":"Dental services available.",
    "orthopedic":"Bone specialist available.",
    "gynecologist":"Women specialist available.",
    "pediatrician":"Child specialist available.",
    "neurologist":"Brain specialist available.",
    "dermatologist":"Skin doctor available.",

    # Emergency
    "emergency":"Call ambulance 108.",
    "ambulance":"Dial 108.",
    "icu":"ICU available 24/7.",
    "oxygen":"Available.",
    "ventilator":"Available.",

    # Lab
    "report":"Download online.",
    "lab test":"Available.",
    "blood test":"Available.",
    "xray":"Available.",
    "mri":"Available.",
    "ct scan":"Available.",

    # Facilities
    "pharmacy":"Open 24/7.",
    "room":"Private and general rooms.",
    "ac room":"Available.",
    "parking":"Available.",
    "canteen":"Available.",
    "wifi":"Free WiFi.",
    "working hours":"Our hospital is open 24/7 for emergency services. Outpatient departments (OPD) are typically open from 9:00 AM to 6:00 PM, Monday to Saturday.",
    " hospital located":"We are located in the main city area. You can use Google Maps or ask for directions through this chatbot.",
    "departments and services ":"We offer services in general medicine, cardiology, orthopedics, pediatrics, gynecology, dermatology, diagnostics, and more.",
    " emergency services available" : "Yes, our emergency department operates 24 hours a day, 7 days a week.",
    "Which doctors are available for [specialty]": "We have experienced doctors in all major specialties. Please specify your required specialty, and I’ll show available doctors.",
    "diagnostic tests":"We provide blood tests, X-rays, MRI, CT scans, ultrasound, ECG, and more.",

"test prescription":"Some tests require a doctor’s prescription, while basic tests can be done directly.",

"test preparation":"Preparation depends on the test. For example, fasting may be required for certain blood tests.",

"test results":"Most reports are available within 24–48 hours. Some tests may take longer.",

"treatment options":"Treatment depends on your diagnosis. Doctors will recommend the best plan after consultation.",

"procedure cost":"Costs vary depending on the procedure. You can request a detailed estimate.",

"recovery time":"Recovery time depends on the treatment or surgery. Your doctor will guide you.",

"treatment risks":"Every treatment has some risks. Doctors will explain all risks before proceeding.",

"payment methods":"We accept cash, credit/debit cards, UPI, and online payments.",

"insurance":"We accept most major insurance providers. Please share your insurer’s name to confirm.",

"cost estimate":"You can request a cost estimate through this chatbot or at the billing desk.",

"cashless insurance":"Yes, cashless treatment is available for selected insurance providers.",

"medical emergency":"Please call our emergency number or visit the hospital immediately.",

"ambulance service":"Yes, ambulance service is available 24/7.",

"emergency care":"Emergency care is provided immediately upon arrival.",

"admission process":"Admission can be done through a doctor’s recommendation or via emergency.",

"room types":"We offer general wards, semi-private rooms, and private rooms.",

"facilities":"Facilities include meals, nursing care, pharmacy, Wi-Fi, and waiting areas.",

"medical reports":"You can access reports through our patient portal or request them via this chatbot.",

"followup appointment":"Yes, follow-up appointments can be scheduled through this chatbot.",

"appointment reminders":"Yes, we send SMS or app notifications as reminders.",

"visiting hours":"Visiting hours are from 4:00 PM to 7:00 PM.",

"required documents":"You need valid ID proof, doctor’s prescription, and insurance documents if applicable.",

"customer support":"You can contact us via helpline, email, or this chatbot.",

    # General
    "hello":"Hello! How can I help?",
    "hi":"Hi there!",
    "thanks":"You're welcome!",
    "bye":"Take care!"
}

# expand to 100+
extra = {}
for key in list(qa.keys()):
    extra[key+" info"] = qa[key]
    extra["tell me "+key] = qa[key]
    extra["details of "+key] = qa[key]

qa.update(extra)

def get_response(msg):
    msg = msg.lower()
    for key in qa:
        if key in msg:
            return qa[key]
    return "Sorry, I can answer hospital queries only."

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/chatbot.html")
def chatbot():
    return render_template("chatbot.html")

@app.route("/chat", methods=["POST"])
def chat():
    user_msg = request.json["message"]
    return jsonify({"reply": get_response(user_msg)})

if __name__ == "__main__":
    app.run(debug=True)