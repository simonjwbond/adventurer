#!/usr/bin/env python3
"""Generate detailed 48x64 human sprite with walk cycle animation."""

WIDTH = 48
HEIGHT = 64

def generate_walk_frame(y, frame):
    """Generate a single row with walk cycle leg movement based on frame (0-2)."""
    row = [0] * WIDTH
    
    # === HAIR (rows 0-4) - Same for all frames ===
    if y == 0:
        w = 20
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 3
    elif y == 1:
        w = 24
        start = (WIDTH - w) // 2
        for x in range(start, min(start + w, WIDTH)):
            row[x] = 3 if x % 2 == 0 else 4
    elif y <= 3:
        hair_left = 7 - y
        hair_right = WIDTH - 7 + y
        for x in range(max(0, hair_left), min(hair_left + 6, WIDTH)):
            row[x] = 3 if (x + y) % 2 == 0 else 4
        for x in range(max(0, hair_right - 6), min(WIDTH, hair_right)):
            row[x] = 3 if (x + y) % 2 == 0 else 4
    
    # === FACE (rows 5-16) - Same for all frames ===
    elif y <= 16:
        outline_x = 7 + (y - 5) // 5
        if outline_x < WIDTH and outline_x >= 0:
            row[outline_x] = 5
        if WIDTH - 1 - outline_x >= 0 and WIDTH - 1 - outline_x < WIDTH:
            row[WIDTH - 1 - outline_x] = 5
        
        # Eyes at rows 9-11
        if y >= 9 and y <= 11:
            eye_center = WIDTH // 2
            left_eye_start = eye_center - 10
            for i, val in enumerate([5, 10, 10, 11, 10, 5]):
                idx = left_eye_start + i
                if 0 <= idx < WIDTH: row[idx] = val
            
            right_eye_start = eye_center + 4
            for i, val in enumerate([5, 10, 10, 11, 10, 5]):
                idx = right_eye_start + i
                if 0 <= idx < WIDTH: row[idx] = val
            
            for x in range(left_eye_start + 6, right_eye_start):
                if row[x] == 0:
                    row[x] = 2 if (x + y) % 3 == 0 else 1
        else:
            face_width = 24
            start = (WIDTH - face_width) // 2
            for x in range(start, min(start + face_width, WIDTH)):
                if row[x] == 0:
                    row[x] = 2 if (x + y) % 3 == 0 else 1
        
        # Nose at rows 13-14
        if y >= 13 and y <= 14:
            nose_center = WIDTH // 2
            for x in range(nose_center - 1, nose_center + 2):
                if row[x] == 0:
                    row[x] = 2
        
        # Mouth at rows 15-16
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
        
        if y == 19:
            for x in range(start, min(start + shoulder_width, WIDTH)):
                row[x] = 6
            collar_center = WIDTH // 2
            row[collar_center - 2] = 5
            row[collar_center - 1] = 7
            row[collar_center] = 9
            row[collar_center + 1] = 7
            row[collar_center + 2] = 5
        else:
            for x in range(start, min(start + shoulder_width, WIDTH)):
                row[x] = 6
    
    # === TORSO WITH ARMS (rows 21-38) - Arms swing opposite to legs ===
    elif y <= 38:
        body_width = max(22, 28 - (y - 21) // 5)
        start = (WIDTH - body_width) // 2
        
        for x in range(start, min(start + body_width, WIDTH)):
            row[x] = 7 if (x + y) % 4 == 0 else 6
        
        # Zipper line down center
        if y >= 21 and y <= 35:
            zipper_center = WIDTH // 2
            row[zipper_center - 1] = 9
            row[zipper_center] = 9
        
        # Arms swing opposite to legs (frame-based)
        if y >= 23 and y <= 36:
            arm_y_offset = y - 23
            
            # Arm swing offset based on frame (0, 1, or 2)
            # Frame 0: left arm forward, right arm back
            # Frame 1: arms neutral  
            # Frame 2: left arm back, right arm forward
            if frame == 0:
                arm_swing = -1 + arm_y_offset // 5
            elif frame == 1:
                arm_swing = arm_y_offset // 5
            else:  # frame == 2
                arm_swing = 1 + arm_y_offset // 5
            
            left_arm_start = start - 5 + arm_swing
            for x in range(left_arm_start, min(left_arm_start + 5, WIDTH)):
                row[x] = 7 if (x + y) % 4 == 0 else 6
            
            right_arm_start = start + body_width - arm_swing
            for x in range(max(0, right_arm_start - 5), min(right_arm_start + 1, WIDTH)):
                row[x] = 7 if (x + y) % 4 == 0 else 6
            
            # Hands at end of arms
            if y >= 34 and y <= 36:
                hand_y_offset = y - 34
                
                left_hand_x = left_arm_start - 2 + hand_y_offset // 2
                for x in range(left_hand_x, min(left_hand_x + 4, WIDTH)):
                    row[x] = 1 if (x + y) % 3 == 0 else 2
                
                right_hand_x = right_arm_start - 2 - hand_y_offset // 2
                for x in range(max(0, right_hand_x), min(right_hand_x + 4, WIDTH)):
                    row[x] = 1 if (x + y) % 3 == 0 else 2
    
    # === LEGS WITH WALK CYCLE (rows 39-52) - Frame-based leg positions ===
    elif y <= 52:
        # Walk cycle offset based on frame
        # Frame 0: left leg forward, right leg back
        # Frame 1: legs together (mid-step)
        # Frame 2: left leg back, right leg forward
        if frame == 0:
            leg_offset = -2  # Left forward
        elif frame == 1:
            leg_offset = 0   # Neutral
        else:  # frame == 2
            leg_offset = 2   # Right forward
        
        leg_spacing = 10 + leg_offset
        leg_width = 6
        
        left_leg_start = WIDTH // 2 - leg_spacing - leg_width
        right_leg_start = WIDTH // 2 + leg_spacing
        
        # Left leg with shading
        row[left_leg_start - 1] = 5
        for x in range(left_leg_start, min(left_leg_start + leg_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
        
        # Right leg with shading  
        row[right_leg_start + leg_width] = 5
        for x in range(right_leg_start, min(right_leg_start + leg_width, WIDTH)):
            row[x] = 8 if (x + y) % 4 == 0 else 6
    
    # === FEET WITH WALK CYCLE (rows 53-58) - Frame-based foot positions ===
    else:
        # Foot offset matches leg offset
        if frame == 0:
            foot_offset = -2
        elif frame == 1:
            foot_offset = 0
        else:  # frame == 2
            foot_offset = 2
        
        foot_spacing = 10 + foot_offset
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

# Generate all frames
for frame in range(3):
    print(f"=== BODY_DATA_FRAME_{frame} ===")
    for y in range(HEIGHT):
        row = generate_walk_frame(y, frame)
        print(",".join(map(str, row)) + ",")
