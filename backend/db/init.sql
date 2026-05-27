CREATE EXTENSION IF NOT EXISTS "pgcrypto";

CREATE TYPE user_role AS ENUM (
    'participant',
    'organizer',
    'moderator'
);

CREATE TYPE event_status AS ENUM (
    'draft',
    'pending',
    'approved',
    'rejected'
);

CREATE TYPE event_source AS ENUM (
    'manual',
    'parsed'
);

CREATE TYPE indexing_type AS ENUM (
    'RSCI',
    'VAK',
    'Scopus',
    'WoS',
    'none'
);

CREATE TYPE application_status AS ENUM (
    'new',
    'approved',
    'rejected'
);

CREATE TYPE parser_status AS ENUM (
    'active',
    'paused',
    'error'
);

CREATE TYPE participation_status AS ENUM (
    'registered',   
    'attended',     
    'awarded',      
    'cancelled'     
);

CREATE TABLE users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email VARCHAR(255) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role user_role NOT NULL DEFAULT 'participant',
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    middle_name VARCHAR(100),
    university VARCHAR(255),
    department VARCHAR(255),
    academic_degree VARCHAR(100),
    is_verified BOOLEAN NOT NULL DEFAULT FALSE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE user_verifications (
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    code VARCHAR(6) NOT NULL,
    expires_at TIMESTAMP NOT NULL
);

CREATE TABLE scientific_fields (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    code VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE events (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    title VARCHAR(500) NOT NULL,
    description TEXT,
    organizer_id UUID REFERENCES users(id) ON DELETE SET NULL,
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    submission_deadline DATE,
    field_id INT REFERENCES scientific_fields(id) ON DELETE SET NULL,
    indexing indexing_type NOT NULL DEFAULT 'none',
    is_free BOOLEAN NOT NULL DEFAULT TRUE,
    participation_fee DECIMAL(10, 2),
    is_online BOOLEAN NOT NULL DEFAULT FALSE,
    location VARCHAR(500),
    requirements_text TEXT,
    info_letter_path VARCHAR(500),
    source_url VARCHAR(1000) CONSTRAINT unique_source_url UNIQUE,
    status event_status NOT NULL DEFAULT 'draft',
    source event_source NOT NULL DEFAULT 'manual',
    rejection_reason TEXT,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    CONSTRAINT check_dates CHECK (start_date <= end_date)
);

CREATE TABLE applications (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    event_id UUID NOT NULL REFERENCES events(id) ON DELETE CASCADE,
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    paper_title VARCHAR(500) NOT NULL,
    file_path VARCHAR(500),
    file_name VARCHAR(255),
    status application_status NOT NULL DEFAULT 'new',
    rejection_reason TEXT,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    CONSTRAINT unique_application UNIQUE (event_id, user_id)
);

CREATE TABLE user_registrations (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    event_id UUID NOT NULL REFERENCES events(id) ON DELETE CASCADE,
    status participation_status NOT NULL DEFAULT 'registered',
    registered_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    CONSTRAINT unique_user_event_reg UNIQUE (user_id, event_id)
);

CREATE TABLE favorites (
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    event_id UUID NOT NULL REFERENCES events(id) ON DELETE CASCADE,
    added_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    PRIMARY KEY (user_id, event_id)
);

CREATE TABLE certificates (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    event_id UUID REFERENCES events(id) ON DELETE SET NULL, 
    title VARCHAR(255) NOT NULL,                     
    file_path VARCHAR(500) NOT NULL,
    file_name VARCHAR(255) NOT NULL,
    uploaded_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE parsers_config (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    target_url VARCHAR(1000) NOT NULL,
    university_name VARCHAR(255),
    status parser_status NOT NULL DEFAULT 'active',
    schedule_cron VARCHAR(100) DEFAULT '0 3 * * *',
    last_run_at TIMESTAMPTZ,
    last_run_status VARCHAR(50),
    error_log TEXT,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE action_logs (
    id BIGSERIAL PRIMARY KEY,
    moderator_id UUID REFERENCES users(id) ON DELETE SET NULL,
    action_type VARCHAR(100) NOT NULL,
    target_type VARCHAR(50) NOT NULL,
    target_id VARCHAR(255),
    details JSONB,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_events_status ON events (status);
CREATE INDEX idx_events_indexing ON events (indexing);
CREATE INDEX idx_events_field ON events (field_id);
CREATE INDEX idx_events_dates ON events (start_date, end_date);
CREATE INDEX idx_events_is_free ON events (is_free);

CREATE INDEX idx_applications_event ON applications (event_id);
CREATE INDEX idx_applications_user ON applications (user_id);
CREATE INDEX idx_applications_status ON applications (status);

CREATE INDEX idx_user_reg_user ON user_registrations (user_id);
CREATE INDEX idx_certificates_user ON certificates (user_id);

CREATE INDEX idx_action_logs_moderator ON action_logs (moderator_id);
CREATE INDEX idx_action_logs_type ON action_logs (action_type);
CREATE INDEX idx_action_logs_created ON action_logs (created_at DESC);

CREATE OR REPLACE FUNCTION update_updated_at_column() RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_users_updated_at BEFORE UPDATE ON users
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER trg_events_updated_at BEFORE UPDATE ON events
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER trg_applications_updated_at BEFORE UPDATE ON applications
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER trg_user_registrations_updated_at BEFORE UPDATE ON user_registrations
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

INSERT INTO scientific_fields (name, code) VALUES 
    ('Прикладная информатика', 'CS-AI'),
    ('Информационная безопасность', 'CS-IS'),
    ('Математика', 'MATH'),
    ('Физика', 'PHYS'),
    ('Химия', 'CHEM'),
    ('Биология', 'BIO'),
    ('История', 'HIST'),
    ('Экономика', 'ECON'),
    ('Юриспруденция', 'LAW'),
    ('Педагогика', 'PED');

INSERT INTO users (email, password_hash, role, first_name, last_name, university) VALUES
    (
        'moderator@sci_events.com',
        '$2b$12$placeholder_hash_here',
        'moderator',
        'Татьяна',
        'Иванова',
        'УрФУ'
    );