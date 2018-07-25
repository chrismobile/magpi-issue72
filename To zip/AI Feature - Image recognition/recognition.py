#!/usr/bin/python
import httplib, urllib, base64, json, re

# CHANGE {MS_API_KEY} BELOW WITH YOUR MICROSOFT VISION API KEY
ms_api_key = "{MS_API_KEY}"

# setup vision API
headers = {
    'Content-Type': 'application/octet-stream',
    'Ocp-Apim-Subscription-Key': ms_api_key,
}
params = urllib.urlencode({
    'visualFeatures': 'Description',
})

# read image
body = open('/tmp/image.jpg', "rb").read()

# submit request to API and print description if successful or error otherwise
try:
    conn = httplib.HTTPSConnection('westcentralus.api.cognitive.microsoft.com')
    conn.request("POST", "/vision/v1.0/analyze?%s"%params, body, headers)
    response = conn.getresponse()
    analysis=json.loads(response.read())
    image_caption = analysis["description"]["captions"][0]["text"].capitalize()
    conn.close()
    print image_caption

except Exception as e:
    print e.args