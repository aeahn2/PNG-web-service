from flask import Flask, render_template, send_file, request, make_response
import os

global image_counter
image_counter = 0

app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  return render_template("index.html")


# Extract a hidden file from the 'file' chunk in PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_file():
  # Create 'temp' directory to cache PNG/Files 
  try:
    os.makedirs("temp")
  except FileExistsError:
    pass

  # Save inputted PNG file into temp directory 
  png_input = request.files['png']
  png_filename = png_input.filename
  png_filepath = f"temp/{png_filename}"
  png_input.save(png_filepath)
  
  # Extract file using png_microservice 
  global image_counter
  extract_filepath = f"temp/{image_counter}"
  image_counter += 1
  os.system("make")
  exit_status = os.system(f"./png-extract {png_filepath} {extract_filepath}")

  if exit_status == 0: #successfully extracted file
    os.remove(png_filepath)
    return send_file(extract_filepath), 200

  if exit_status == 512: #invalid PNG
    os.remove(png_filepath)
    image_counter -= 1
    response = make_response("Error: invalid PNG", 422)
    return response

  if exit_status == 1024: #valid PNG but missing 'file' chunk
    os.remove(png_filepath)
    os.remove(extract_filepath)
    image_counter -= 1
    response = make_response("Error: no hidden file to extract", 415)
    return response


# Get the nth saved extracted file from 'temp/' cache:
@app.route('/extract/<int:image_num>', methods=['GET'])
def extract_image(image_num):
  global image_counter
  if image_num >= image_counter:
    response = make_response(f"Error: Image #{image_num} not found", 404)
    return response
  image_filepath = f"temp/{image_num}"
  return send_file(image_filepath), 200
