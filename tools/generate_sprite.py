#!/usr/bin/env python3
"""Generate 48x64 pixel art sprite data for the hero character."""

WIDTH = 48
HEIGHT = 64

# Palette indices:
# 0 = transparent
# 1 = skin (peach)
# 2 = hair (blonde)  
# 3 = outline (dark)
# 4 = eye white
# 5 = pupil (black)

def generate_down_sprite():
    """Generate down-facing hero sprite with blonde hair and blue jumpsuit."""
    data = []
    
    for y in range(HEIGHT):
        row = [0] * WIDTH
        
        # Hair top (rows 0-4) - messy blonde hair
        if y <= 3:
            hair_width = 16 + y * 2
            start_x = (WIDTH - hair_width) // 2
            for x in range(start_x, start_x + hair_width):
                row[x] = 2
        
        # Hair framing face (rows 4-7)
        elif y <= 7:
            hair_width = 20 + y * 2
            start_x = (WIDTH - hair_width) // 2
            for x in range(start_x, start_x + hair_width):
                row[x] = 2
            # Face area
            face_start = start_x + 4
            face_end = start_x + hair_width - 4
            for x in range(face_start, face_end):
                if y < 6:
                    row[x] = 1  # forehead skin
        
        # Face with eyes (rows 8-13)
        elif y <= 13:
            # Outline on sides
            outline_left = 4 + y // 2
            outline_right = WIDTH - 4 - y // 2
            row[outline_left] = 3
            row[outline_right] = 3
            
            if y >= 9 and y <= 11:
                # Eyes at rows 9-11
                eye_x_left = WIDTH // 2 - 8
                eye_x_right = WIDTH // 2 + 8
                
                # Left eye
                row[eye_x_left] = 3      # outline
                row[eye_x_left + 1] = 4  # white
                row[eye_x_left + 2] = 5  # pupil
                row[eye_x_left + 3] = 4  # white
                row[eye_x_left + 4] = 3  # outline
                
                # Right eye  
                row[eye_x_right - 4] = 3
                row[eye_x_right - 3] = 4
                row[eye_x_right - 2] = 5
                row[eye_x_right - 1] = 4
                row[eye_x_right] = 3
                
                # Skin between eyes
                for x in range(eye_x_left + 5, eye_x_right - 5):
                    row[x] = 1
            else:
                # Just skin (chin area)
                face_width = 20
                start_x = (WIDTH - face_width) // 2
                for x in range(start_x, start_x + face_width):
                    row[x] = 1
        
        # Neck and shoulders (rows 14-20)
        elif y <= 20:
            outline_left = 3 + (y - 14)
            outline_right = WIDTH - 3 - (y - 14)
            row[outline_left] = 3
            row[outline_right] = 3
            
            # Skin for neck/shoulders
            for x in range(outline_left + 1, outline_right):
                row[x] = 1
        
        # Torso body (rows 21-40) - blue jumpsuit covers this
        elif y <= 40:
            outline_left = 3 + (y - 21) // 2
            outline_right = WIDTH - 3 - (y - 21) // 2
            row[outline_left] = 3
            row[outline_right] = 3
            
            # Body color (will be overridden by clothing layer)
            for x in range(outline_left + 1, outline_right):
                row[x] = 1
        
        # Legs (rows 41-60) - jumpsuit continues down
        elif y <= 60:
            leg_width = max(8, 20 - (y - 40))
            start_x = (WIDTH - leg_width) // 2
            outline_left = start_x
            outline_right = start_x + leg_width
            row[outline_left] = 3
            row[outline_right - 1] = 3
            
            for x in range(outline_left + 1, outline_right - 1):
                row[x] = 1
        
        # Feet (rows 61-63)
        else:
            foot_width = max(4, 10 - (y - 60) * 2)
            start_x = (WIDTH - foot_width) // 2
            for x in range(start_x, start_x + foot_width):
                row[x] = 1
        
        data.extend(row)
    
    return data

def generate_clothing_tunic():
    """Generate blue jumpsuit clothing layer."""
    data = []
    
    for y in range(HEIGHT):
        row = [0] * WIDTH
        
        # Jumpsuit starts at shoulders (row 21) and goes to feet
        if y >= 21:
            outline_left = 3 + (y - 21) // 2 if y <= 40 else 3 + (y - 40) // 3
            outline_right = WIDTH - 3 - (y - 21) // 2 if y <= 40 else WIDTH - 3 - (y - 40) // 3
            
            # Add collar detail at top
            if y == 21:
                collar_center = WIDTH // 2
                row[collar_center - 2] = 6  # zipper start
                row[collar_center - 1] = 6
                row[collar_center] = 6
                row[collar_center + 1] = 6
                row[collar_center + 2] = 6
            
            # Add zipper line down center
            if y > 21 and y < 45:
                row[WIDTH // 2 - 1] = 6
                row[WIDTH // 2] = 6
            
            for x in range(outline_left + 1, outline_right):
                row[x] = 7  # blue jumpsuit color
        
        data.extend(row)
    
    return data

# Generate and print the arrays
print("=== BODY_DATA (48x64) ===")
body_data = generate_down_sprite()
for y in range(HEIGHT):
    row = body_data[y * WIDTH:(y + 1) * WIDTH]
    print(",".join(map(str, row)) + ",")

print("\n=== CLOTHING_TUNIC (48x64) ===")
cloth_data = generate_clothing_tunic()
for y in range(HEIGHT):
    row = cloth_data[y * WIDTH:(y + 1) * WIDTH]
    print(",".join(map(str, row)) + ",")
