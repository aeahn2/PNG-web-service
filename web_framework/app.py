from flask import Flask, render_template, send_file, request, make_response
import os

global image_counter
image_counter = 0

app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  return render_template("index.html")


# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  print("In extract_hidden_gif()")
  # Create 'temp' directory to hold PNG/GIFs 
  try:
    os.makedirs("temp")
  except FileExistsError:
    pass

  # Save inputted PNG file into temp directory 
  png_input = request.files['png']
  png_filename = png_input.filename
  png_filepath = f"temp/{png_filename}"
  png_input.save(png_filepath)
  
  # Extract GIF using MP2 
  global image_counter
  gif_filepath = f"temp/{image_counter}.gif"
  image_counter += 1
  os.system("make")
  exit_status = os.system(f"./png-extractGIF {png_filepath} {gif_filepath}")

  if exit_status == 0: #successfully extracted GIF
    os.remove(png_filepath)
    return send_file(gif_filepath), 200

  if exit_status == 512: #invalid PNG
    os.remove(png_filepath)
    image_counter -= 1
    response = make_response("Error: invalid PNG", 422)
    return response

  if exit_status == 1024: #valid PNG but missing 'uiuc' chunk
    os.remove(png_filepath)
    os.remove(gif_filepath)
    image_counter -= 1
    response = make_response("Error: no hidden GIF image to extract", 415)
    return response


# Get the nth saved "uiuc" GIF:
@app.route('/extract/<int:image_num>', methods=['GET'])
def extract_image(image_num):
  global image_counter
  if image_num >= image_counter:
    response = make_response(f"Error: Image #{image_num} not found", 404)
    return response
  image_filepath = f"temp/{image_num}.gif"
  return send_file(image_filepath), 200
