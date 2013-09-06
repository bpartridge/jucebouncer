import urllib2
import json

def loadJSON(presetNumber=0):
  data = json.dumps({'presetNumber': presetNumber})
  response = urllib2.urlopen("http://localhost:8080/list.json", data)
  responseText = response.read()
  return responseText

if __name__ == '__main__':
  print loadJSON()