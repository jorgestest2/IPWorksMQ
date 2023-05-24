#
# IPWorks MQ 2022 Python Edition - Demo Application
#
# Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
#

import sys
import string
from ipworksmq import *

input = sys.hexversion<0x03000000 and raw_input or input


def fireCharacters(e):
  print(e.text)

def fireStartDoc(e):
  print("Started parsing file")

def fireEndDoc(e):
  print("Finished parsing file")

def fireError(e):
  print(e.message)


def fireStartElement(e):
  print("Element %s started"%e.element)

def fireEndElement(e):
  print("Element %s ended"%e.element)

json = JSON()

json.on_characters = fireCharacters
json.on_end_document = fireEndDoc
json.on_end_element = fireEndElement
json.on_error = fireError
json.on_start_document = fireStartDoc
json.on_start_element = fireStartElement

try:
  json.set_input_file('books.json')
  json.parse()
  json.x_path = "/json/store/books"
  bookCount = json.get_x_child_count()
  propCount = 0
  for x in range(1,bookCount+1):
    print("\r\nBook #" + str(x))
    json.x_path = "/json/store/books/[" + str(x) + "]"
    propCount = json.get_x_child_count()
    for y in range(1,propCount+1):
      json.x_path = "/json/store/books/[" + str(x) + "]/[" + str(y) + "]"
      print(json.x_element + ": " + json.x_text)
except IPWorksMQError as e:
    print("ERROR: %s"%e.message)

