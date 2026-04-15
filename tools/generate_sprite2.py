#!/usr/bin/env python3
"""Generate proper 48x64 pixel art for the hero - blonde hair, blue jumpsuit."""

WIDTH = 48
HEIGHT = 64

def generate_row(y):
    """Generate a single row of the down-facing sprite."""
    row = [0] * WIDTH
    
    # Hair (rows 0-7) - messy blonde on top
    if y <= 2:
        # Top of hair
        w = 18 + y * 3
        start = (WIDTH - w) // 2
        for x in range(start, start + w):
            row[x] = 2  # blonde hair
    elif y <= 5:
        # Hair framing face
        w = 24 + y * 2
        start = (WIDTH - w) // 2
        for x in range(start, start + w):
            row[x] = 2
        # Leave face area clear (skin will be drawn below)
    elif y <= 7:
        # Hair sides only
        hair_w = 6
        left_start = 10 - y
        right_start = WIDTH - 10 + y
        for x in range(left_start, left_start + hair_w):
            row[x] = 2
        for x in range(right_start - hair_w, right_start):
            row[x] = 2
    
    # Face (rows 8-15)
    elif y <= 15:
        outline_x = 6 + (y - 8) // 2
        row[outline_x] = 3  # dark outline left
        row[WIDTH - 1 - outline_x] = 3  # dark outline right
        
        if y >= 10 and y <= 12:
            # Eyes at rows 10-12
            eye_pos = WIDTH // 2
            # Left eye
            row[eye_pos - 8] = 3
            row[eye_pos - 7] = 4  # white
            row[eye_pos - 6] = 5  # pupil  
            row[eye_pos - 5] = 4
            row[eye_pos - 4] = 3
            # Right eye
            row[eye_pos + 4] = 3
            row[eye_pos + 5] = 4
            row[eye_pos + 6] = 5
            row[eye_pos + 7] = 4
            row[eye_pos + 8] = 3
        else:
            # Just skin (chin/forehead)
            face_w = 20
            start = (WIDTH - face_w) // 2
            for x in range(start, start + face_w):
                if row[x] == 0:  # Don't overwrite hair
                    row[x] = 1  # skin
    
    # Neck and shoulders (rows 16-22)
    elif y <= 22:
        outline_x = 8 + (y - 16) // 2
        row[outline_x] = 3
        row[WIDTH - 1 - outline_x] = 3
        for x in range(outline_x + 1, WIDTH - 1 - outline_x):
            if row[x] == 0:
                row[x] = 1
    
    # Torso (rows 23-42) - body shape
    elif y <= 42:
        outline_x = 10 + (y - 23) // 4
        row[outline_x] = 3
        row[WIDTH - 1 - outline_x] = 3
        for x in range(outline_x + 1, WIDTH - 1 - outline_x):
            if row[x] == 0:
                row[x] = 1
    
    # Legs (rows 43-58)
    elif y <= 58:
        leg_w = max(6, 16 - (y - 43) // 2)
        start = (WIDTH - leg_w * 2) // 2
        row[start] = 3
        row[start + leg_w * 2 - 1] = 3
        for x in range(start + 1, start + leg_w * 2 - 1):
            if row[x] == 0:
                row[x] = 1
    
    # Feet (rows 59-63)
    else:
        foot_w = max(4, 8 - (y - 59))
        start = (WIDTH - foot_w * 2) // 2
        for x in range(start, start + foot_w * 2):
            if row[x] == 0:
                row[x] = 1
    
    return row

# Generate all rows
print("=== BODY_DATA ===")
for y in range(HEIGHT):
    row = generate_row(y)
    print(",".join(map(str, row)) + ",")
