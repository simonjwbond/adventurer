#!/usr/bin/env python3
"""Generate detailed 48x64 human sprite with proper pixel art shading and details."""

WIDTH = 48
HEIGHT = 64

# Palette:
# 0 = transparent
# 1 = skin light (peach)
# 2 = skin shadow (darker peach)
# 3 = hair blonde
# 4 = hair shadow (golden brown)
# 5 = outline dark (brown/black)
# 6 = clothing blue main
# 7 = clothing blue highlight
# 8 = clothing blue shadow
# 9 = zipper gold
# 10 = eye white
# 11 = pupil black
# 12 = mouth red

def generate_detailed_human_row(y, frame=0):
    """Generate a single row of the down-facing human sprite with details."""
    row = [0] * WIDTH
    
    # === HAIR (rows 0-4) - Messy blonde hair with shading ===
    if y == 0:
        w = 20
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 3  # blonde
    elif y == 1:
        w = 24
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 3 if x % 2 == 0 else 4  # alternating blonde/shadow
    elif y <= 3:
        hair_left = 7 - y
        hair_right = WIDTH - 7 + y
        for x in range(max(0, hair_left), min(hair_left + 6, WIDTH)):
            row[x] = 3 if (x + y) % 2 == 0 else 4
        for x in range(max(0, hair_right - 6), min(WIDTH, hair_right)):
            row[x] = 3 if (x + y) % 2 == 0 else 4
    
    # === FACE (rows 5-16) - Detailed face with features ===
    elif y <= 16:
        outline_x = 7 + (y - 5) // 5
        if outline_x < WIDTH and outline_x >= 0:
            row[outline_x] = 5
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 5
        
        # Eyes at rows 9-11 (detailed with whites, pupils, shading)
        if y >= 9 and y <= 11:
            eye_center = WIDTH // 2
            
            # Left eye (6 pixels wide)
            left_eye_start = eye_center - 10
            row[left_eye_start] = 5           # outline
            row[left_eye_start + 1] = 10      # white
            row[left_eye_start + 2] = 10      # white
            row[left_eye_start + 3] = 11      # pupil
            row[left_eye_start + 4] = 10      # white
            row[left_eye_start + 5] = 5       # outline
            
            # Right eye (6 pixels wide)
            right_eye_start = eye_center + 4
            row[right_eye_start] = 5
            row[right_eye_start + 1] = 10
            row[right_eye_start + 2] = 10
            row[right_eye_start + 3] = 11
            row[right_eye_start + 4] = 10
            row[right_eye_start + 5] = 5
            
            # Skin between and around eyes with shading
            for x in range(left_eye_start + 6, right_eye_start):
                if row[x] == 0:
                    row[x] = 2 if (x + y) % 3 == 0 else 1
        else:
            # Just skin with subtle shading
            face_width = 24
            start = (WIDTH - face_width) // 2
            for x in range(start, min(start + face_width, WIDTH)):
                if row[x] == 0:
                    row[x] = 2 if (x + y) % 3 == 0 else 1
        
        # Nose at rows 13-14 (subtle shading)
        if y >= 13 and y <= 14:
            nose_center = WIDTH // 2
            for x in range(nose_center - 1, nose_center + 2):
                if row[x] == 0:
                    row[x] = 2
        
        # Mouth at rows 15-16 (red lips)
        if y >= 15 and y <= 16:
            mouth_center = WIDTH // 2
            for x in range(mouth_center - 3, mouth_center + 4):
                row[x] = 12
    
    # === NECK (rows 17-18) ===
    elif y <= 18:
        neck_left = 19
        neck_right = 29
        for x in range(neck_left, min(neck_right + 1, WIDTH)):
            row[x] = 2 if (x + y) % 3 == 0 else 1
    
    # === COLLAR/SHOULDERS (rows 19-20) ===
    elif y <= 20:
        shoulder_width = 34
        start = (WIDTH - shoulder_width) // 2
        
        # Collar area with V-neck detail
        if y == 19:
            for x in range(start, min(start + shoulder_width, WIDTH)):
                row[x] = 6  # blue clothing
            # V-neck collar outline
            collar_center = WIDTH // 2
            row[collar_center - 2] = 5
            row[collar_center - 1] = 7  # highlight
            row[collar_center] = 9      # zipper start
            row[collar_center + 1] = 7
            row[collar_center + 2] = 5
        else:
            for x in range(start, min(start + shoulder_width, WIDTH)):
                row[x] = 6
    
    # === TORSO WITH ARMS (rows 21-38) - Blue jumpsuit with details ===
    elif y <= 38:
        body_width = max(22, 28 - (y - 21) // 5)
        start = (WIDTH - body_width) // 2
        
        # Main torso with shading
        for x in range(start, min(start + body_width, WIDTH)):
            row[x] = 7 if (x + y) % 4 == 0 else 6  # highlight pattern
        
        # Zipper line down center (rows 21-35)
        if y >= 21 and y <= 35:
            zipper_center = WIDTH // 2
            row[zipper_center - 1] = 9
            row[zipper_center] = 9
        
        # Arms at sides with hands (rows 23-36)
        if y >= 23 and y <= 36:
            arm_y_offset = y - 23
            
            # Left arm extending from shoulder
            left_arm_start = start - 5 + arm_y_offset // 5
            for x in range(left_arm_start, min(left_arm_start + 5, WIDTH)):
                row[x] = 7 if (x + y) % 4 == 0 else 6
            
            # Right arm
            right_arm_start = start + body_width - arm_y_offset // 5
            for x in range(max(0, right_arm_start - 5), min(right_arm_start + 1, WIDTH)):
                row[x] = 7 if (x + y) % 4 == 0 else 6
            
            # Hands at end of arms (rows 34-36)
            if y >= 34 and y <= 36:
                hand_y_offset = y - 34
                
                # Left hand
                left_hand_x = left_arm_start - 2 + hand_y_offset // 2
                for x in range(left_hand_x, min(left_hand_x + 4, WIDTH)):
                    row[x] = 1 if (x + y) % 3 == 0 else 2
                
                # Right hand
                right_hand_x = right_arm_start - 2 - hand_y_offset // 2
                for x in range(max(0, right_hand_x), min(right_hand_x + 4, WIDTH)):
                    row[x] = 1 if (x + y) % 3 == 0 else 2
    
    # === LEGS (rows 39-52) - Two separate legs with shading ===
    elif y <= 52:
        leg_spacing = 10
        leg_width = 6
        
        left_leg_start = WIDTH // 2 - leg_spacing - leg_width
        right_leg_start = WIDTH // 2 + leg_spacing
        
        # Left leg with shading
        row[left_leg_start - 1] = 5   # outline
        for x in range(left_leg_start, min(left_leg_start + leg_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
        
        # Right leg with shading  
        row[right_leg_start + leg_width] = 5  # outline
        for x in range(right_leg_start, min(right_leg_start + leg_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
    
    # === FEET (rows 53-58) - Detailed boots/shoes ===
    else:
        foot_spacing = 10
        foot_width = 8
        
        left_foot_start = WIDTH // 2 - foot_spacing - foot_width + 3
        right_foot_start = WIDTH // 2 + foot_spacing - 3
        
        # Left foot (boot style)
        for x in range(left_foot_start, min(left_foot_start + foot_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
        
        # Right foot (boot style)
        for x in range(right_foot_start, min(right_foot_start + foot_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
    
    return row

# Generate all rows for frame 0
print("=== BODY_DATA_FRAME_0 ===")
for y in range(HEIGHT):
    row = generate_detailed_human_row(y, frame=0)
    print(",".join(map(str, row)) + ",")
