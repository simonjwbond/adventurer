#!/usr/bin/env python3
"""Generate proper 48x64 human sprite - compact proportions with visible face and arms."""

WIDTH = 48
HEIGHT = 64

def generate_compact_human_row(y):
    """Generate a single row of the down-facing human sprite.
    Palette: 0=transparent, 1=skin, 2=hair (blonde), 3=outline, 4=eye_white, 5=pupil, 6=mouth
    """
    row = [0] * WIDTH
    
    # === HAIR (rows 0-3) - Messy blonde hair ===
    if y <= 0:
        w = 18
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y == 1:
        w = 22
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y <= 3:
        # Hair framing face sides
        hair_left = 8 - y
        hair_right = WIDTH - 8 + y
        for x in range(max(0, hair_left), min(hair_left + 5, WIDTH)):
            row[x] = 2
        for x in range(max(0, hair_right - 5), min(WIDTH, hair_right)):
            row[x] = 2
    
    # === FACE (rows 4-13) - Compact face with features ===
    elif y <= 13:
        outline_x = 8 + (y - 4) // 4
        if outline_x < WIDTH and outline_x >= 0:
            row[outline_x] = 3
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 3
        
        # Eyes at rows 7-9 (center of face)
        if y >= 7 and y <= 9:
            eye_center = WIDTH // 2
            # Left eye (4 pixels wide)
            left_eye_start = eye_center - 9
            row[left_eye_start] = 3       # outline
            row[left_eye_start + 1] = 4   # white
            row[left_eye_start + 2] = 5   # pupil  
            row[left_eye_start + 3] = 3   # outline
            
            # Right eye (4 pixels wide)
            right_eye_start = eye_center + 5
            row[right_eye_start] = 3
            row[right_eye_start + 1] = 4
            row[right_eye_start + 2] = 5
            row[right_eye_start + 3] = 3
            
            # Skin between eyes
            for x in range(left_eye_start + 4, right_eye_start):
                if row[x] == 0:
                    row[x] = 1
        else:
            # Just skin (forehead/chin)
            face_width = 22
            start = (WIDTH - face_width) // 2
            for x in range(start, min(start + face_width, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
        
        # Mouth at rows 11-12
        if y >= 11 and y <= 12:
            mouth_center = WIDTH // 2
            for x in range(mouth_center - 2, mouth_center + 3):
                if row[x] == 0:
                    row[x] = 6  # mouth color (dark)
    
    # === NECK (rows 14-15) ===
    elif y <= 15:
        neck_left = 20
        neck_right = 28
        for x in range(neck_left, min(neck_right + 1, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # === SHOULDERS (row 16) ===
    elif y == 16:
        shoulder_width = 32
        start = (WIDTH - shoulder_width) // 2
        for x in range(start, min(start + shoulder_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # === TORSO WITH ARMS (rows 17-35) ===
    elif y <= 35:
        body_width = max(20, 26 - (y - 17) // 4)
        start = (WIDTH - body_width) // 2
        
        # Main torso
        for x in range(start, min(start + body_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
        
        # Arms at sides (rows 18-32)
        if y >= 18 and y <= 32:
            arm_y_offset = y - 18
            arm_width = 4
            
            # Left arm
            left_arm_start = start - arm_width + arm_y_offset // 4
            for x in range(left_arm_start, min(left_arm_start + arm_width, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
            
            # Right arm  
            right_arm_start = start + body_width - arm_y_offset // 4
            for x in range(max(0, right_arm_start - arm_width), min(right_arm_start + 1, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
    
    # === LEGS (rows 36-52) ===
    elif y <= 52:
        leg_spacing = 8
        leg_width = 5
        
        left_leg_start = WIDTH // 2 - leg_spacing - leg_width
        right_leg_start = WIDTH // 2 + leg_spacing
        
        # Left leg
        row[left_leg_start - 1] = 3   # outline
        for x in range(left_leg_start, min(left_leg_start + leg_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
        
        # Right leg  
        row[right_leg_start + leg_width] = 3  # outline
        for x in range(right_leg_start, min(right_leg_start + leg_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # === FEET (rows 53-58) ===
    else:
        foot_spacing = 8
        foot_width = 7
        
        left_foot_start = WIDTH // 2 - foot_spacing - foot_width + 2
        right_foot_start = WIDTH // 2 + foot_spacing - 2
        
        # Left foot
        for x in range(left_foot_start, min(left_foot_start + foot_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
        
        # Right foot
        for x in range(right_foot_start, min(right_foot_start + foot_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    return row

# Generate all rows
print("=== BODY_DATA ===")
for y in range(HEIGHT):
    row = generate_compact_human_row(y)
    print(",".join(map(str, row)) + ",")
