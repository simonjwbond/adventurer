#!/usr/bin/env python3
"""Generate proper 48x64 human sprite with blonde hair and face features."""

WIDTH = 48
HEIGHT = 64

def generate_human_row(y):
    """Generate a single row of the down-facing human sprite.
    Palette: 0=transparent, 1=skin, 2=hair (blonde), 3=outline, 4=eye_white, 5=pupil
    """
    row = [0] * WIDTH
    
    # === HAIR (rows 0-8) - Messy blonde hair on top ===
    if y <= 1:
        # Top of hair
        w = 20 + y * 4
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y <= 5:
        # Full hair coverage
        w = 28 + y * 3
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 2
    elif y <= 8:
        # Hair framing face sides
        hair_left_start = 8 - y
        hair_right_start = WIDTH - 8 + y
        for x in range(max(0, hair_left_start), min(hair_left_start + 6, WIDTH)):
            row[x] = 2
        for x in range(max(0, hair_right_start - 6), min(WIDTH, hair_right_start)):
            row[x] = 2
    
    # === FACE (rows 9-18) ===
    elif y <= 18:
        outline_x = 8 + (y - 9) // 3
        if outline_x < WIDTH:
            row[outline_x] = 3
        if WIDTH - 1 - outline_x >= 0:
            row[WIDTH - 1 - outline_x] = 3
        
        # Eyes at rows 12-14
        if y >= 12 and y <= 14:
            eye_center = WIDTH // 2
            # Left eye (5 pixels wide)
            left_eye_start = eye_center - 10
            row[left_eye_start] = 3       # outline
            row[left_eye_start + 1] = 4   # white
            row[left_eye_start + 2] = 5   # pupil  
            row[left_eye_start + 3] = 4   # white
            row[left_eye_start + 4] = 3   # outline
            
            # Right eye (5 pixels wide)
            right_eye_start = eye_center + 6
            row[right_eye_start] = 3
            row[right_eye_start + 1] = 4
            row[right_eye_start + 2] = 5
            row[right_eye_start + 3] = 4
            row[right_eye_start + 4] = 3
            
            # Skin between and around eyes
            for x in range(left_eye_start + 5, right_eye_start):
                if row[x] == 0:
                    row[x] = 1
        else:
            # Just skin (forehead/chin)
            face_width = 24
            start = (WIDTH - face_width) // 2
            for x in range(start, min(start + face_width, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
    
    # === NECK (rows 19-21) ===
    elif y <= 21:
        neck_left = 18
        neck_right = 30
        row[neck_left - 1] = 3   # outline left
        row[neck_right + 1] = 3  # outline right
        for x in range(neck_left, min(neck_right + 1, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # === SHOULDERS/TORSO (rows 22-45) - Body shape ===
    elif y <= 45:
        body_width = max(16, 28 - (y - 22) // 3)
        start = (WIDTH - body_width) // 2
        row[start - 1] = 3   # outline left
        row[start + body_width] = 3  # outline right
        
        # Arms at sides (rows 25-38)
        if y >= 25 and y <= 38:
            arm_y_offset = y - 25
            arm_left = start - 6 + arm_y_offset // 2
            arm_right = start + body_width + 6 - arm_y_offset // 2
            
            # Draw arms
            for x in range(arm_left, min(arm_left + 4, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
            for x in range(max(0, arm_right - 3), min(arm_right + 1, WIDTH)):
                if row[x] == 0:
                    row[x] = 1
        
        # Fill torso with skin (will be covered by clothing)
        for x in range(start, min(start + body_width, WIDTH)):
            if row[x] == 0:
                row[x] = 1
    
    # === LEGS (rows 46-58) ===
    elif y <= 58:
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
    
    # === FEET (rows 59-63) ===
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
    row = generate_human_row(y)
    print(",".join(map(str, row)) + ",")
