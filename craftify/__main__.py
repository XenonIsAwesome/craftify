import os
import subprocess
import concurrent.futures
import argparse
from PIL import Image
from tqdm import tqdm


def extract_frames(video_path: str, output_folder: str):
    print(f"Extracting frames from {video_path} to {output_folder}")

    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    subprocess.run(['ffmpeg', 
                    '-i', video_path, 
                    f'{output_folder}/frame_%04d.png'], 
                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def stitch_frames_to_video(frames_folder: str, output_video_path: str, original_video_path: str):
    print(f"Stitching frames from {frames_folder} to {output_video_path} w/ audio from {original_video_path}")

    try:
        os.remove(output_video_path)
    except FileNotFoundError:
        pass
    
    subprocess.run(['ffmpeg', 
                    '-framerate', '30', 
                    '-i', f'{frames_folder}/frame_%04d.png', 
                    '-c:v', 'libx264', 
                    '-pix_fmt', 'yuv420p', 
                    output_video_path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    # Extract audio from the original video
    audio_path = '/tmp/temp_audio.aac'
    
    try:
        os.remove(audio_path)
    except FileNotFoundError:
        pass

    subprocess.run(['ffmpeg', 
                    '-i', original_video_path, 
                    '-vn', '-acodec', 'copy', 
                    audio_path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    # Combine the new video with the extracted audio
    temp_output_video_path = '/tmp/temp_output.mp4'

    try:
        os.remove(temp_output_video_path)
    except FileNotFoundError:
        pass
    
    subprocess.run(['ffmpeg', 
                    '-i', output_video_path, 
                    '-i', audio_path, 
                    '-c:v', 'copy', 
                    '-c:a', 'aac', 
                    '-strict', 'experimental', 
                    temp_output_video_path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    # Replace the output video with the combined video
    os.replace(temp_output_video_path, output_video_path)

    # Clean up temporary audio file
    os.remove(audio_path)


def process_frames(frames_folder: str, output_folder: str, mode: str = "normal", scale_factor: int = 1, max_threads: int = 100):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    frame_files = sorted([f for f in os.listdir(frames_folder) if f.endswith('.png')])

    def process_frame(frame_file):
        frame_path = os.path.join(frames_folder, frame_file)
        converted_img_path = os.path.join(output_folder, frame_file)

        subprocess.run(['build/craftify_convert_frame', frame_path, converted_img_path, mode, str(scale_factor)], 
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    with concurrent.futures.ThreadPoolExecutor(max_workers=max_threads) as executor:
        list(tqdm(executor.map(process_frame, frame_files), total=len(frame_files), desc="Converting frames to minecraft blocks"))


def combine_in_out_frames(
    frames_folder_path: str, 
    output_frames_folder_path: str, 
    combined_frames_folder: str, 

    combined_video_path: str, 
    original_video_path: str,

    max_threads: int = 100
):
    # Create the combined frames folder if it doesn't exist
    os.makedirs(combined_frames_folder, exist_ok=True)

    # Get the list of frames in the input and output frames folders
    frames_folder = sorted(os.listdir(frames_folder_path))
    output_frames_folder = sorted(os.listdir(output_frames_folder_path))

    def process_frame(pre_conversion_frame_filename, post_conversion_frame_filename):
        # Construct the full paths for the pre-conversion and post-conversion frames
        pre_conversion_frame_path = os.path.join(frames_folder_path, pre_conversion_frame_filename)
        post_conversion_frame_path = os.path.join(output_frames_folder_path, post_conversion_frame_filename)
        combined_frames_path = os.path.join(combined_frames_folder, pre_conversion_frame_filename)

        # Open the pre-conversion and post-conversion frames
        pre_conversion_frame = Image.open(pre_conversion_frame_path)
        post_conversion_frame = Image.open(post_conversion_frame_path)

        # Calculate the dimensions for the combined frame
        combined_width = pre_conversion_frame.width + post_conversion_frame.width
        combined_height = max(pre_conversion_frame.height, post_conversion_frame.height)
        
        # Create a new image with the combined dimensions
        combined_frame = Image.new('RGBA', (combined_width, combined_height), (0, 0, 0))

        # Paste the pre-conversion frame on the left side of the combined frame
        # and the post-conversion frame on the right side of it
        combined_frame.paste(pre_conversion_frame, (0, 0))
        combined_frame.paste(post_conversion_frame, (pre_conversion_frame.width, 0))

        # Save the combined frame to the specified path
        combined_frame.save(combined_frames_path)

    with concurrent.futures.ThreadPoolExecutor(max_workers=max_threads) as executor:
        list(tqdm(executor.map(process_frame, frames_folder, output_frames_folder), total=len(frames_folder), desc="Combining frames"))

    # Stitch the combined frames into a video
    stitch_frames_to_video(combined_frames_folder, combined_video_path, original_video_path)


def cleanup_temp_folder(folder_path: str):
    for file in os.listdir(folder_path):
        try:
            os.remove(os.path.join(folder_path, file))
        except:
            pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process videos into minecraft blocks.')
    parser.add_argument('-i', '--input', required=True, help='Path to the input video file.')
    parser.add_argument('-c', '--combined', required=False, help='Path to the combined output video file.')
    parser.add_argument('--scale-factor', type=int, default=1, help='Scale factor for frame processing.')
    parser.add_argument('--max-threads', type=int, default=100, help='Maximum number of threads for frame processing.')
    parser.add_argument('-m', '--mode', default="normal", help='normal/lamp.')
    parser.add_argument('--play-output', action='store_true', help='Play the output video after processing.')
    parser.add_argument('output', help='Path to the output video file.')

    args = parser.parse_args()

    video_path = args.input
    combined_video_path = args.combined
    scale_factor = args.scale_factor
    max_threads = args.max_threads
    output_video_path = args.output
    mode = args.mode
    play_output = args.play_output

    frames_folder = '/tmp/frames'
    output_frames_folder = '/tmp/output_frames'
    combined_frames_folder = '/tmp/combined_frames'

    try:
        cleanup_temp_folder(frames_folder)
    except:
        pass

    try:
        cleanup_temp_folder(output_frames_folder)
    except:
        pass
    
    try:
        cleanup_temp_folder(combined_frames_folder)
    except:
        pass

    if scale_factor > 2:
        print("This might take a while, grab a coffee.")

    extract_frames(video_path, frames_folder)
    process_frames(frames_folder, output_frames_folder, mode=mode, scale_factor=scale_factor, max_threads=max_threads)
    stitch_frames_to_video(output_frames_folder, output_video_path, video_path)
    
    print(f"Output video saved to {output_video_path}")
    result_video = output_video_path

    if combined_video_path:
        combine_in_out_frames(frames_folder, output_frames_folder, combined_frames_folder, combined_video_path, video_path, max_threads)
        print(f"Combined video saved to {combined_video_path}")

        result_video = combined_video_path

    if play_output:
        subprocess.run(['ffplay', result_video], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
