-- ═══════════════════════════════════════════════════════════════
--   ENARM Database - Datos semilla (005)
-- ═══════════════════════════════════════════════════════════════

-- Usuario demo
INSERT INTO users (email, password_hash, full_name, role, email_verified)
VALUES (
    'demo@enarm.local',
    crypt('demo123', gen_salt('bf')),
    'Estudiante Demo',
    'student',
    TRUE
)
ON CONFLICT (email) DO NOTHING;

-- Estudiante demo
INSERT INTO students (user_id, student_code, university, current_level, target_specialty)
SELECT id, 'DEMO-001', 'UNAM', 'aspirante_enarm', 'Medicina Interna'
FROM users WHERE email = 'demo@enarm.local'
ON CONFLICT (student_code) DO NOTHING;

-- Perfil del estudiante demo
INSERT INTO student_profiles (student_id, preferred_difficulty, daily_study_goal_minutes)
SELECT s.id, 'intermediate', 90
FROM students s
JOIN users u ON s.user_id = u.id
WHERE u.email = 'demo@enarm.local'
ON CONFLICT (student_id) DO NOTHING;

-- Guias clinicas mexicanas
INSERT INTO clinical_guidelines (id, title, organization, year, url) VALUES
    ('IMSS-234-09', 'Diagnostico y Tratamiento del Infarto Agudo del Miocardio con Elevacion del ST', 'IMSS', 2019, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-162-09', 'Diagnostico y Tratamiento del Sindrome Coronario Agudo sin Elevacion del ST', 'IMSS', 2020, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-014-08', 'Diagnostico y Tratamiento de la Hipertension Arterial en el Primer Nivel de Atencion', 'IMSS', 2020, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-718-14', 'Diagnostico y Tratamiento de la Neumonia Adquirida en la Comunidad', 'IMSS', 2019, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-104-08', 'Diagnostico y Tratamiento de la Diabetes Mellitus tipo 2', 'IMSS', 2018, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-020-08', 'Diagnostico y Tratamiento de la Enfermedad Renal Cronica Temprana', 'IMSS', 2020, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-085-08', 'Diagnostico y Tratamiento de la Sepsis y Choque Septico', 'IMSS', 2021, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-320-10', 'Diagnostico y Tratamiento del Asma en Ninos y Adultos', 'IMSS', 2019, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-037-08', 'Diagnostico y Tratamiento de la Enfermedad Pulmonar Obstructiva Cronica', 'IMSS', 2018, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-058-08', 'Diagnostico y Tratamiento del Evento Vascular Cerebral Isquemico Agudo', 'IMSS', 2019, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-706-14', 'Diagnostico y Tratamiento de la Preeclampsia-Eclampsia', 'IMSS', 2020, 'http://www.cenetec.salud.gob.mx'),
    ('IMSS-032-08', 'Diagnostico y Tratamiento de la Cetoacidosis Diabetica en Adultos', 'IMSS', 2019, 'http://www.cenetec.salud.gob.mx')
ON CONFLICT (id) DO NOTHING;